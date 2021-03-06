/**
 * SpillStateMachineSMB.H
 *
 * This file contains the definition of the SpillStateMachineSMB class.
 *
 * @author jdiamond and rtadkins
 */

#ifndef SPILLSTATEMACHINESMB_H
#define SPILLSTATEMACHINESMB_H

#include "ssm.H"
#include "CircularBuffer.H"

namespace Mu2eER
{
  /**
   * SpillStateMachineSMB
   *
   * Shared memory block for the spill state machine.
   */
  class SpillStateMachineSMB
  {
  public:
    /**
     * Constructor
     */
    SpillStateMachineSMB();

    /**
     * Initialize
     */
    void initialize();

    /**
     * Get Circular Buffer
     *
     * @return Circular buffer of the spill state machine
     */
    const CircularBuffer<double>& circularBufferGet() const;
    
    // CircularBuffer<CircBuffer>& circularBufferStructGet();
    
    /**
     * Set Current State
     *
     * @param state Current spill state machine state
     */
    void currentStateSet( ssm_state_t state );

    /**
     * Get Current State
     *
     * @return Current state of the spill state machine
     */
    ssm_state_t currentStateGet() const;

    /**
     * Get LED State
     *
     * @return The whether the LED is on or off
     */
    bool ledStateGet() const;

    /**
     * Set LED State
     *
     * Sets the state of the LED.
     */
    void ledStateSet( bool ledState );

    /**
     * Get Spill Counter
     *
     * @return Spill counter
     */
    unsigned int spillCounterGet() const;

    /**
     * Set Spill Counter
     *
     * @param sc New spill counter value
     */
    void spillCounterSet( unsigned int sc );

    /**
     * Set Thread Running Flag
     *
     * @param running Set to true when the SSM thread is running, false when it's not
     */
    void threadRunningSet( bool running );

    /**
     * Is SSM Thread Running
     *
     * @return True if the SSM thread is running
     */
    bool threadRunningGet() const;

    /**
     * Get Time-in-Spill
     *
     * @return Time spent in the last spill cycle (ms)
     */
    float timeInSpillGet() const;

    /**
     * Set Time-in-Spill
     *
     * @param tis New time-in-spill value
     */
    void timeInSpillSet( float tis );

    /**
     * Get Ideal Spill Data Array
     *
     * @return Array Holding Data
     */
    const int* idealSpillWaveFormGet() const;

    /**
     * Get Ideal Spill Data Array Size
     *
     * @return Size of Array
     */
    int idealSpillWaveFormSizeGet() const;

    /**
     * Transform Data Into Ideal Spill Waveform
     */
    void idealSpillWaveform();

    /**
     * Get Actual Spill Data Array
     *
     * @return Array Holding Data
     */
    const int* actualSpillWaveFormGet() const;

    /**
     * Get Actual Spill Data Array Size
     *
     * @return Size of Array
     */
    int actualSpillWaveFormSizeGet() const;

    /**
     * Transform Data Into Actual Spill Waveform
     */
    void actualSpillWaveform();

    /**
     * Get Error Signal Data Array
     *
     * @return Array Holding Data
     */
    const int* errorSignalWaveFormGet() const;

    /**
     * Get Error Signal Data Array Size
     *
     * @return Size of Array
     */
    int errorSignalWaveFormSizeGet() const;

    /**
     * Transform Data Into Error Spill Waveform
     */
    void errorSignalWaveform();

    /**
     * Fill Circular Buffer
     */
    void fillCircularBuffer();

  private:
    /**
    * Circular buffer
    */
    CircularBuffer<double> _circular_buffer{9800};

    /**
     * Current state
     */
    ssm_state_t _currentState;

    /**
     * Number of spills since the SSM was initialized
     */
    unsigned int _spillCounter;

    /**
     * True if the spill state machine thread is running
     */
    bool _threadRunning;

    /**
     * Time spent in the last spill cycle (ms)
     */
    float _timeInSpill;

    /**
     * LED
     */
    bool _led;

    /**
     * Size of array
     */
    int _idealSpillWaveFormSize = 16000;

    /**
     * Ideal Spill Data array
     */
    int _idealSpillWaveFormData[16000];

    /**
     * Actual Spill Data array
     */
    int _actualSpillWaveFormData[16000] = {0};

    /**
     * Error Spill Data array
     */
    int _errorSignalWaveFormData[16000] = {0};
  };
};

#endif
