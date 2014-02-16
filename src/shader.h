#ifndef SHADER_H
#define SHADER_H

#include <map>
#include <utility>

typedef std::map<const char*,GLuint> ShaderIdMap;

typedef std::map<GLuint,const char*> ShaderTypeNameMap;
typedef std::pair<GLuint,const char*> ShaderTypeName;


class Shader {
    public:
        GLuint program_id;

        static Shader* getShader(const ShaderTypeNameMap &shader_names);

        Shader(){};
        explicit Shader(const ShaderTypeNameMap &shader_names);

    private:
        static std::map<ShaderTypeName,GLuint> shader_ids;
        static std::map<ShaderTypeNameMap,Shader> shader_programs;

        GLuint addShader(ShaderTypeName &shader_type_name);
        const char* loadSource(const char* file_name);
};

#endif
