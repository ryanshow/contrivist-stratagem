#ifndef SHADER_H
#define SHADER_H

#include <map>
#include <utility>

typedef std::map<const char*,GLuint> ShaderIdMap;

typedef std::map<GLuint,const char*> ShaderTypeNameMap;
typedef std::pair<GLuint,const char*> ShaderTypeName;

class Shader {
    public:
        GLuint mProgramId;

        Shader(){};
        explicit Shader(const ShaderTypeNameMap& shader_names);

        static Shader* getShader(const ShaderTypeNameMap& shader_names);

    private:
        static std::map<ShaderTypeName,GLuint>      msShaderIdMap;
        static std::map<ShaderTypeNameMap,Shader>   msShaderProgramMap;

        GLuint          addShader(const ShaderTypeName & shader_type_name);
        const char *    loadSource(const char * file_name);
};

#endif
