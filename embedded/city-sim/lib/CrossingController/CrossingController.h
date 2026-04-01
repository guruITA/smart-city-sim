#ifndef CROSSING_CONTROLLER_H
#define CROSSING_CONTROLLER_H

#include "TrainDetector.h"
#include "SignalController.h"
#include "CommunicationHandler.h"

class CrossingController {
public:
  CrossingController(TrainDetector detector,
                     SignalController signalController,
                     CommunicationHandler comm);

  void begin();
  void update();

private:
  TrainDetector _detector;
  SignalController _signal;
  CommunicationHandler _comm;

  int _trainId;
};

#endif