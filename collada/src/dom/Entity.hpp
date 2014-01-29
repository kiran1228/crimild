#ifndef CRIMILD_COLLADA_ENTITY_
#define CRIMILD_COLLADA_ENTITY_

#include "Utils.hpp"

namespace crimild {

	namespace collada {

		class Entity : public SharedObject {
		public:
			virtual ~Entity( void );

			inline void setID( std::string id ) { _id = id; }
			inline const char *getID( void ) const { return _id.c_str(); }

			virtual bool parseXML( xmlNode *input );

		protected:
			Entity( void );

		private:
			std::string _id;
		};

	}

}

#endif

