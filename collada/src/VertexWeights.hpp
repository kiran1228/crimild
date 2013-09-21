#ifndef CRIMILD_COLLADA_VERTEX_WEIGHTS_
#define CRIMILD_COLLADA_VERTEX_WEIGHTS_

#include "Entity.hpp"
#include "EntityLibrary.hpp"
#include "Input.hpp"

namespace crimild {

	namespace collada {

		class VertexWeights : public Entity {
		public:
			VertexWeights( void );

			virtual ~VertexWeights( void );

			bool parseXML( xmlNode *input );

			inline unsigned int getIndexCount( void ) const { return _indices.size(); }
			inline const int *getIndices( void ) const { return ( _indices.size() > 0 ? &_indices[ 0 ] : NULL ); }

		private:
			std::vector< int > _indices;
		};

		typedef std::shared_ptr< VertexWeights > VertexWeightsPtr;

	}

}

#endif

