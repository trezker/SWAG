#include "controller.h"

Controller::~Controller() {
}

void Controller::Process_event(const Event& event) {
	Events::iterator i = events.find(event);
	if(i != events.end()) {
		Handle_event(i->second, event);
	}
}
