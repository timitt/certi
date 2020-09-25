#ifndef MOMFEDERATEAMBASSADOR1516E_H
#define MOMFEDERATEAMBASSADOR1516E_H

#include "momfederateambassador.h"

using namespace rti1516e;

class MOMFederateAmbassador1516e : public MOMFederateAmbassador {
public:
    MOMFederateAmbassador1516e(RTIambassador& ambassador,
                          const std::wstring& federation_name,
                          const std::wstring& federate_name,
                          const bool is_auto = false,
                          const int report_period = 1,
                          const int report_style = 0
                         ) : MOMFederateAmbassador(ambassador, federate_name, federate_name, is_auto, report_period, report_style) {}

    virtual ~MOMFederateAmbassador1516e() = default;
}

#endif
