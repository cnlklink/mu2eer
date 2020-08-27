/**
 * ControlMQClient.H
 *
 * This file contains the definition of the ControlMQClient class.
 *
 * @author jdiamond
 */

#ifndef CONTROLMQCLIENT_H
#define CONTROLMQCLIENT_H

#include <mqueue.h>
#include <string>

#include "api.H"

using namespace std;

namespace Mu2eER
{
  /**
   * ControlMQClient
   *
   * This class implements an interface around the mu2eerd Control Message Queue ("/mu2eer") that 
   * clients can use to send messages to mu2eerd without having to construct POSIX messages on 
   * their own.
   */
  class ControlMQClient
  {
  public:
    /**
     * Constructor
     *
     * @param mqName Name of mu2eerd message queue to attach to
     */
    ControlMQClient( string mqName );

    /**
     * Destructor
     */
    ~ControlMQClient();

    /**
     * Shutdown
     *
     * Sends CMD_SHUTDOWN to bpmd.
     */
    void shutdown();

    /**
     * Initialize Spill State Machine
     *
     * Prepare for slow spill regulation.
     */
    void ssmInit();

    /**
     * Reset the Spill State Machine
     *
     * Reset slow spill regulation.
     */
    void reset();

    /**
     * Start the Spill State Machine
     *
     * Start the slow spill regulation.
     */
    void start();

    /**
     * Send an Invalid Command
     *
     * Sends a message with an invalid command.  Used for testing only.
     */
    void testBadCommand();

  private:
    /**
     * Message Queue
     */
    mqd_t _mq;

    /**
     * Message queue name
     */
    string _mqName;

    /**
     * Send Message
     *
     * @param msg Message
     */
    void _send( control_msg_t& msg );
  };
};

#endif
