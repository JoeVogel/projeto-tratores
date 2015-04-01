'use strict';

var express = require('express'),
    app = express(),
    http = require('http').Server(app),
    bodyParser = require('body-parser'),
    errorHandler = require('errorhandler'),
    methodOverride = require('method-override'),
    path = require('path'),
    db = require('./models'),
    passport = require('passport'),
    flash = require('connect-flash'),
    LocalStrategy = require('passport-local').Strategy,
    user = require('./routes/user'),
    testtype = require('./routes/testtype'),
    sensor = require('./routes/sensor'),
    equipment = require('./routes/equipment'),
    review = require('./routes/review'),
    formula = require('./routes/formula'),
    sensordata = require('./routes/sensordata'),
    cases = require('./routes/case'),
    route_passport = require('./routes/passport'),
    io = require('socket.io')(http),
    client = null;

app.set('port', process.env.PORT || 3000)
app.use(bodyParser())
app.use(express.static(path.join(__dirname, 'public')))

app.configure(function() {
  app.use(express.logger());
  app.use(express.cookieParser());
  app.use(express.methodOverride());
  app.use(express.session({ secret: 'new-holand-project' }));
  app.use(flash());
  app.use(passport.initialize());
  app.use(passport.session());
  app.use(app.router);
});

if ('development' === app.get('env')) {
  app.use(errorHandler())
}

function isAuthenticatedPage(req, res, next) {
  if (req.isAuthenticated()){
    return next();
  }
  res.send({ success: 2, message: 'Falha na autenticação!' });
}

function isAuthenticated(req, res, next) {
  if (req.isAuthenticated()){
    return next();
  }
  res.redirect('/');
};

passport.serializeUser(function(user, done) {
  done(null, user.id);
});

passport.deserializeUser(function(id, done) {
  route_passport.findById(id, function (err, user) {
    done(err, user);
  });
});

passport.use(new LocalStrategy(
  function(username, password, done) {
    process.nextTick(function () {
      route_passport.findByEmail(username, password, function(err, user) {
        if (err) {
          return done(err);
        }
        if (user == null) {
          return done(null, null);
        }
        return done(null, user);
      })
    });
  }
));

// GET
app.get('/', function(req, res, next){
  res.sendfile('public/index.html');
});

app.get('/home', isAuthenticated, function(req, res, next){
  res.sendfile('public/home.html');
});

app.get('/logout', function(req, res, next){
  req.logout();
  res.redirect('/');
});

app.get('/views/:page', isAuthenticatedPage, function(req, res, next){
  res.sendfile('public/views/' + req.param('page').split("!")[0] + '/index.html');
});

app.get('/views/persist/:page', isAuthenticatedPage, function(req, res, next){
  res.sendfile('public/views/' + req.param('page').split("!")[0] + '/persist/index.html');
});

app.get('/views/realtime/:page', isAuthenticatedPage, function(req, res, next){
  res.sendfile('public/views/' + req.param('page').split("!")[0] + '/realtime/index.html');
});

app.get('/views/excel/:page', isAuthenticatedPage, function(req, res, next){
  res.sendfile('public/views/' + req.param('page').split("!")[0] + '/excel/index.html');
});

app.get('/user', isAuthenticatedPage, function(req, res, next){
  res.json({
    name: req.user.name,
    email: req.user.email
  });
});

app.get('/data-view-formula', isAuthenticatedPage, formula.dataView)
app.get('/data-view-review', isAuthenticatedPage, review.dataView)
app.get('/data-view-sensor', isAuthenticatedPage, sensor.dataView)
app.get('/data-view-sensordata', isAuthenticatedPage, sensordata.dataView)
app.get('/data-view-sensordata-excel', isAuthenticatedPage, sensordata.dataViewExcel)
app.get('/data-view-sensordata-review/:id', isAuthenticatedPage, sensordata.dataViewReview)

app.get('/testtype', isAuthenticatedPage, testtype.findAll)
app.get('/testtype/:id', isAuthenticatedPage, testtype.find)
app.get('/equipment', isAuthenticatedPage, equipment.findAll)
app.get('/equipment/:id', isAuthenticatedPage, equipment.find)
app.get('/case', isAuthenticatedPage, cases.findAll)
app.get('/case/:id', isAuthenticatedPage, cases.find)
app.get('/sensor', isAuthenticatedPage, sensor.findAll)
app.get('/sensor/:id', isAuthenticatedPage, sensor.find)
app.get('/formula', isAuthenticatedPage, formula.findAll)
app.get('/formula/:id', isAuthenticatedPage, formula.find)
app.get('/formula-review/:id', isAuthenticatedPage, formula.findFormulas)
app.get('/sensordata', isAuthenticatedPage, sensordata.find)
app.get('/sensordata-pdf', isAuthenticatedPage, sensordata.findAll)
app.get('/review-real-time', isAuthenticatedPage, review.findRealTime)
app.get('/excel/review/:id', isAuthenticatedPage, review.createExcel)

// POST
app.post('/login',
  passport.authenticate('local', {
    failureRedirect: '/',
    failureFlash: true
  }), function(req, res, next) {
    res.json({ success: 1})
});

app.post('/user', isAuthenticatedPage, user.persist)
app.post('/testtype', isAuthenticatedPage, testtype.persist)
app.post('/equipment', isAuthenticatedPage, equipment.persist)
app.post('/sensor', isAuthenticatedPage, sensor.persist)
app.post('/case', isAuthenticatedPage, cases.persist)
app.post('/formula', isAuthenticatedPage, formula.persist)
app.post('/review', isAuthenticatedPage, review.persist)
app.post('/review-1/:id', isAuthenticatedPage, review.init)
app.post('/review-2/:id', isAuthenticatedPage, review.close)
app.post('/review-3/:id', isAuthenticatedPage, review.cancel)

// DELETE
app.delete('/testtype/:id', isAuthenticatedPage, testtype.delete)
app.delete('/equipment/:id', isAuthenticatedPage, equipment.delete)
app.delete('/sensor/:id', isAuthenticatedPage, sensor.delete)
app.delete('/case/:id', isAuthenticatedPage, cases.delete)
app.delete('/formula/:id', isAuthenticatedPage, formula.delete)

// Inicialização
db.sequelize.sync({ force: false }).complete(function(err) {
  if (err) {
    throw err
  } else {

    user.init();
    //review.createExcel();

    http.listen(app.get('port'), function(){

      io.on('connection', function (socket) {

        socket.on('data', function(obj){
          socket.broadcast.emit('_processing');
          sensordata.persist(obj);
        });


        socket.on('isClient', function () {
          if(client){
            socket.emit('_isClient', true);
          }else{
            socket.emit('_isClient', false);
          }
        });

        socket.on('init', function (conf) {
          socket.conf = conf;
          client = socket;
          db.Review.find({
            where: {
              status: 1
            },
            attributes: ['id']
          }).success(function(entity) {
            if (entity) {
              client.emit(client.conf.coletor, entity.id);
              socket.broadcast.emit('_status', 1);
            }
          });
        });

        socket.on('status', function (idReview) {
          if(client){
            client.emit(client.conf.coletor, idReview);
            io.sockets.emit('_status', 1);
          }
        });

        socket.on('findData', function (obj) {
          db.Sensor.find({
            where: {
              id: obj.sensor
            },
            attributes: ['id', 'description', 'field'],
            include: [ {
              model: db.Case,
              attributes: ['id', 'macAddress']
            }],
          }).success(function(entitySensor) {
            if(entitySensor){
              db.SensorData.findAll({
                where: {
                  macAddress: entitySensor.Case.macAddress,
                  ReviewId: obj.review
                },
                attributes: ['id', 'date', 'counter', 'sensor' + entitySensor.field],
                order: 'counter ASC'
              }).success(function(entityData) {
                db.Formula.find({
                  where: {
                    ReviewId: obj.review,
                    SensorId: obj.sensor
                  },
                  attributes: ['formula']
                }).success(function(entityFormula) {
                  if(entityFormula){
                    socket.emit('_findData', sensordata.get(entityData || [], entitySensor, entityFormula.formula));
                  }else{
                    socket.emit('_findData', sensordata.get(entityData || [], entitySensor, ''));
                  }
                });
              });
            }
          });
        });

        socket.on('finish', function(data) {
          socket.broadcast.emit('_finish');
        });

        socket.on('disconnect', function (data) {
          if(socket.conf){
            client = null;
          }
        });
      });

      console.log('Express server listening on port ' + app.get('port'))
    });
  }
})