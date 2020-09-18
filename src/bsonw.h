#ifndef BSONW_H
#define BSONW_H

#include <memory>
#include <bson.h>
#include <mongoc.h>

namespace mongo{

class stBson;
typedef std::shared_ptr<stBson> BsonPtr;

class stBson {
    public:
        stBson(bson_t* t);

		~stBson();

        bool copy( bson_t *t ){ mBson = t; return true; }
    public:
    //追加bson
        bool appendBson( const std::string& key, const stBson& bData );
    //合并bson
        bool merageBson( const stBson &bData );
	//json
		bool initFromJson( const std::string &json );
	
		void printASJson();	
    public:
        bool setString( const std::string& key, const std::string& value );

        bool setInt32( const std::string &key, int32_t value );

        bool setInt64( const std::string &key, int64_t value  );

        bool setBinary( const std::string &key,  const uint8_t *binaryData, uint32_t length );

        bool hasKey( const std::string &key );
    public:
    //判断是否是文档类型
        bool isObject( const std::string&  dotKey );

        BsonPtr asObject( const std::string&  dotKey );

    //判断是否是字符串  
        bool isString( const std::string&  dotKey );

        std::string asString( const std::string&  dotKey);

    //判断是否是uint64
        bool isInt64( const std::string& dotKey ); 

        int64_t asInt64( const std::string& dotKey );

    //判断是否是uint32
        bool isInt32( const std::string& dotKey );

        int32_t asInt32( const std::string& dotKey );

    //判断是否是Double
        bool isDouble( const std::string& dotKey );  

        uint32_t asDouble( const std::string& dotKey );

    //是否是binary
        bool isBinary( const std::string& dotKey );

        bool asBinary( const std::string& dotKey, uint8_t *outData, uint32_t outLen );

    //获取字段类型
        bson_type_t getValueType( const std::string& dotKey );  
    public:
        const bson_t* raw() const { return mBson; }

    private:
        bson_t* mBson = NULL;

        bson_t mChild;
};

};

#endif
