template<typename type>
chai<type>::chai()
{
    // vector3
    script.add(chaiscript::constructor<vector3<type> ()>(), "vector3");
    script.add(chaiscript::constructor<vector3<type> (const vector3<type>&)>(), "vector3");
    script.add(chaiscript::constructor<vector3<type> (type, type, type)>(), "vector3");

    // boolean operation
    script.add(chaiscript::fun(static_cast<void (mesh<type>::*)(const mesh<type>*)>(&mesh<type>::add)), "add");
    script.add(chaiscript::fun(static_cast<void (mesh<type>::*)(const mesh<type>*)>(&mesh<type>::sub)), "sub");
    script.add(chaiscript::fun(static_cast<void (mesh<type>::*)(const mesh<type>*)>(&mesh<type>::inter)), "inter");

    // generate
    script.add(chaiscript::fun([](const vector3<type>& p, const vector3<type>& s) { return generate::box(p, s); }), "box");
    script.add(chaiscript::fun([](const vector3<type>& p, type r) { return generate::sphere(p, r); }), "sphere");
    script.add(chaiscript::fun([](const vector3<type>& p, type r, size_t u, size_t v) { return generate::sphere(p, r, u, v); }), "sphere");
    script.add(chaiscript::fun([](const vector3<type>& p, type h, type l) { return generate::pyramid(p, h, l); }), "pyramid");

    script.add(chaiscript::fun([](const std::string& filename) { return generate::load<type>(filename); }), "load");
}

template<typename type>
chaiscript::ChaiScript& chai<type>::getScript()
{
    return script;
}
