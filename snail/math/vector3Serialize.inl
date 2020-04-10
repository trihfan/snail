#ifdef SNAIL_JSON
namespace Eigen
{
    template <typename type>
    inline void to_json(nlohmann::json& j, const Matrix<type, 3, 1>& v)
    {
        j["x"] = v.x();
        j["y"] = v.y();
        j["z"] = v.z();
    }

    template <typename type>
    inline void from_json(const nlohmann::json& j, Matrix<type, 3, 1>& v)
    {
        j.at("x").get_to(v.x());
        j.at("y").get_to(v.y());
        j.at("z").get_to(v.z());
    }
}
#endif
