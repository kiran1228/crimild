#ifndef CRIMILD_COLLADA_MATRIX_
#define CRIMILD_COLLADA_MATRIX_

#include "Entity.hpp"

namespace crimild {

	namespace collada {

		class Matrix : public Entity {
		public:
			Matrix( void );

			virtual ~Matrix( void );

			bool parseXML( xmlNode *input );

			inline const float *getData( void ) const { return _data; }

		private:
			float _data[ 16 ];
		};

		typedef std::shared_ptr< collada::Matrix > MatrixPtr;

	}

}

#endif
