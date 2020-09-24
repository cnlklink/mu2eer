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
     * Get Data Array Size
     *
     * @return Size of Array
     */
    int actualSpillWaveFormSizeGet() const;

    /**
     * Transform Data Into Actual Spill Waveform
     */
    void actualSpillWaveform();

  private:
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
    int _actualSpillWaveFormData[16000];
  };
};

#endif
