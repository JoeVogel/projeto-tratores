

#include "FreqMeasure.h"
#include "FreqMeasureCapture.h"

#define FREQMEASURE_BUFFER_LEN 12
static volatile uint32_t buffer_value[FREQMEASURE_BUFFER_LEN];
static volatile uint8_t buffer_head;
static volatile uint8_t buffer_tail;
static uint16_t capture_msw;
static uint32_t capture_previous;


void FreqMeasureClass::begin(void)
{
	capture_init();
	capture_msw = 0;
	capture_previous = 0;
	buffer_head = 0;
	buffer_tail = 0;
	capture_start();
}

uint8_t FreqMeasureClass::available(void)
{
	uint8_t head, tail;

	head = buffer_head;
	tail = buffer_tail;
	if (head >= tail) return head - tail;
	return FREQMEASURE_BUFFER_LEN + head - tail;
}

uint32_t FreqMeasureClass::read(void)
{
	uint8_t head, tail;
	uint32_t value;

	head = buffer_head;
	tail = buffer_tail;
	if (head == tail) return 0xFFFFFFFF;
	tail = tail + 1;
	if (tail >= FREQMEASURE_BUFFER_LEN) tail = 0;
	value = buffer_value[tail];
	buffer_tail = tail;
	return value;
}

float FreqMeasureClass::countToFrequency(uint32_t count)
{
#if defined(__AVR__)
	return (float)F_CPU / (float)count;
#elif defined(__arm__) && defined(TEENSYDUINO)
	return (float)F_BUS / (float)count;
#endif
}

void FreqMeasureClass::end(void)
{
	capture_shutdown();
}

#if defined(__AVR__)

ISR(TIMER_OVERFLOW_VECTOR)
{
	capture_msw++;
}

ISR(TIMER_CAPTURE_VECTOR)
{
	uint16_t capture_lsw;
	uint32_t capture, period;
	uint8_t i;

	// get the timer capture
	capture_lsw = capture_read();
	// Handle the case where but capture and overflow interrupts were pending
	// (eg, interrupts were disabled for a while), or where the overflow occurred
	// while this ISR was starting up.  However, if we read a 16 bit number that
	// is very close to overflow, then ignore any overflow since it probably
	// just happened.
	if (capture_overflow() && capture_lsw < 0xFF00) {
		capture_overflow_reset();
		capture_msw++;
	}
	// compute the waveform period
	capture = ((uint32_t)capture_msw << 16) | capture_lsw;
	period = capture - capture_previous;
	capture_previous = capture;
	// store it into the buffer
	i = buffer_head + 1;
	if (i >= FREQMEASURE_BUFFER_LEN) i = 0;
	if (i != buffer_tail) {
		buffer_value[i] = period;
		buffer_head = i;
	}
}

#elif defined(__arm__) && defined(TEENSYDUINO)

void FTM_ISR_NAME (void)
{
	uint32_t capture, period, i;
	bool inc = false;

	if (capture_overflow()) {
		capture_overflow_reset();
		capture_msw++;
		inc = true;
	}
	if (capture_event()) {
		capture = capture_read();
		if (capture <= 0xE000 || !inc) {
			capture |= (capture_msw << 16);
		} else {
			capture |= ((capture_msw - 1) << 16);
		}
		// compute the waveform period
		period = capture - capture_previous;
		capture_previous = capture;
		// store it into the buffer
		i = buffer_head + 1;
		if (i >= FREQMEASURE_BUFFER_LEN) i = 0;
		if (i != buffer_tail) {
			buffer_value[i] = period;
			buffer_head = i;
		}
	}
}


#endif

FreqMeasureClass FreqMeasure;

