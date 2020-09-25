#ifndef CERTI_INTERACTION_1516e_HH
#define CERTI_INTERACTION_1516e_HH

#include "../Interaction.hh"
#include "Parameter1516e.h"

#include <cassert>

namespace certi {


class Interaction1516e : public Interaction
{
public:

    Interaction1516e()=delete;

    /** Create an Interaction.
     * @param[in] name the object class name
     * @param[in] handle the object class handle value
     */
    Interaction1516e(const std::string& name, InteractionClassHandle handle, TransportType transport, OrderType order) : Interaction(name, handle, transport, order){}

    /// Destroy an object class.
    virtual ~Interaction1516e(){}

    void addInheritedClassAttributes(Interaction* the_child)
    {
        for (HandleParameterMap::iterator i = _handleParameterMap.begin(); i != _handleParameterMap.end(); ++i) {
            assert(i->second != NULL);

            Parameter1516e* child = new Parameter1516e(*static_cast<Parameter1516e*>(i->second));
            assert(child != NULL);

            the_child->addParameter(child, true);
        }
    }

};

}
#endif
