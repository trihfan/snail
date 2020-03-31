/*
 *
 */

#pragma once

namespace snail
{
    template <typename type>
    using mesh_t = std::unique_ptr<mesh<type>>;

    template <typename type>
    class mesh
	{
	public:
        /**
         * @brief Constructor
         * @param reserve_size The estimated number of vertices to reserve in the buffers
         */
        mesh(size_t reserveSize = 0);

        virtual ~mesh();

        /**
         * @brief Return a clone of the current mesh
         */
        mesh_t<type> clone() const;

        // vertex
        size_t addVertex(const vector3<type>& vertex);
        void removeVertex(const vector3<type>& vertex);
        void removeVertex(size_t index);
        size_t getVertexCount() const;
        const vector3<type>& getVertex(size_t index) const;

        // triangle
        size_t addTriangle(size_t a, size_t b, size_t c);
        void removeTriangle(const triangle<type>& triangle);
        void removeTriangle(size_t index);
        size_t getTriangleCount() const;
        const triangle<type>& getTriangle(size_t index) const;

        // boolean operations
        void add(mesh_t<type> mesh);
        void sub(mesh_t<type> mesh);
        void inter(mesh_t<type> mesh);

	private:
        /**
         * @brief Enumeration of boolean operations
         */
        enum booleanOperation { difference, addition, intersection };

        /**
         * @brief Array of the mesh vertices
         */
        std::vector<vector3<type>> vertices;

        /**
         * @brief Map used to merge duplicated vertices
         */
        std::unordered_map<vector3<type>, size_t> verticesMap;

        /**
         * @brief Array of the mesh triangles
         */
        std::vector<triangle<type>*> triangles;

        void compute(booleanOperation operation, mesh_t<type> other);

        void cutMesh(mesh<type>* other);
        bool cutTriangle(size_t index, const std::vector<typename triangleTriangleIntersection<type>::intersection>& intersections);

        void mergeMesh(booleanOperation operation, mesh<type>* other);

        void refine();

        static size_t sCurrentIndex;
        size_t id;
	};

    template <typename type>
    size_t mesh<type>::sCurrentIndex = 0;

    #include "mesh.inl"
}
