/*
 *
 */

#pragma once

namespace snail
{
    template <typename type>
    inline size_t currentMeshIndex = 0;

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

        void clear();

        /**
         * @brief Return a clone of the current mesh
         */
        mesh<type>* clone() const;

        // vertex
        size_t addVertex(const vector3<type>& vertex);
        void removeVertex(const vector3<type>& vertex);
        void removeVertex(size_t index);
        size_t getVertexCount() const;
        const vector3<type>& getVertex(size_t index) const;

        // triangle
        size_t addTriangle(size_t a, size_t b, size_t c);
        size_t addTriangle(const vector3<type>& a, const vector3<type>& b, const vector3<type>& c);
        void removeTriangle(const triangle<type>& triangle);
        void removeTriangle(size_t index);
        size_t getTriangleCount() const;
        const triangle<type>& getTriangle(size_t index) const;

        // intersection
        //const bounds<type>& getBounds() const;
        bool isInside(const vector3<type>& point, bool onSideEqualInside);

        // boolean operations
        void add(const mesh<type>* mesh);
        void add(std::unique_ptr<mesh<type>> mesh);

        void sub(const mesh<type>* mesh);
        void sub(std::unique_ptr<mesh<type>> mesh);

        void inter(const mesh<type>* mesh);
        void inter(std::unique_ptr<mesh<type>> mesh);

	private:
        /**
         * @brief Enumeration of boolean operations
         */
        enum  class booleanOperation { difference, addition, intersection };

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

        void compute(booleanOperation operation, std::unique_ptr<mesh<type>> other);

        void cutMesh(mesh<type>* other);
        bool cutTriangle(size_t index, const intersection<type>& intersection);

        void mergeMesh(booleanOperation operation, mesh<type>* other);

        void refine();

        size_t id;
	};



    #include "mesh.inl"
    #include "meshSerialize.inl"
}
