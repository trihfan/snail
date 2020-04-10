/*
 * Class to generate mesh from simple shapes (box, sphere, ...)
 */

#pragma once

namespace snail
{
    template<typename type>
    class chai
    {
    public:
        chai();
        chaiscript::ChaiScript& getScript();

    private:
        chaiscript::ChaiScript script;
    };

    #include "chai.inl"
}
