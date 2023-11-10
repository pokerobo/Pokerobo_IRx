#include "Commons.h"

#if (CONTROLLER_IR)
#include "IR_Controller.h"
#endif

#if (CONTROLLER == CONTROLLER_PS2)
#include "PS2_Controller.h"
#endif

#if (CONTROLLER == CONTROLLER_RF24)
#include "RF24_Controller.h"
#endif

#if (CONTROLLER_CARBOT)
#include "Carbot_Handler.h"
#endif

#include "Pedestal_Group.h"
