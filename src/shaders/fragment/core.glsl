#version 460 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;    
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
};

in vec3 vs_pos;
in vec2 vs_texture_coord;
in vec3 vs_normal;

out vec4 fs_color;

uniform Material material;

uniform vec3 camera_pos;
uniform vec3 light_pos_0;

vec3 norm = normalize(vs_normal);
vec3 light_dir = normalize(light_pos_0 - vs_pos);
vec3 viewDir = normalize(camera_pos - vs_pos);

vec3 calculateAmbientLight(Material material) {
    return material.ambient;
}

vec3 calculateDiffuseLight(Material material) {
    float diffuse_coeficient = clamp(dot(-light_dir, norm), 0, 1);
    return material.diffuse * diffuse_coeficient * texture(material.texture_diffuse1, vs_texture_coord).rgb;
}

vec3 calculateSpecularLight(Material material) {
    vec3 reflect_dir_vec = reflect(-light_dir, norm);
    float specular_coeficient = pow(max(dot(viewDir, reflect_dir_vec), 0), 20);
    return material.specular * specular_coeficient * material.shininess * texture(material.texture_specular1, vs_texture_coord).rgb;
}

void main()
{
    // fs_color = vec4(vs_color, 1.f);
    // fs_color = texture(texture0, vs_texture_coord) + texture(texture1, vs_texture_coord);
    
    // Ambient Light
    vec3 ambient_final = calculateAmbientLight(material);

    // Difuse Light
    vec3 diffuse_final = calculateDiffuseLight(material);

    // Specular Light
    vec3 specular_final = calculateSpecularLight(material);

    // Attenuation
        //TODO

    vec3 light = ambient_final + diffuse_final  + specular_final;

    fs_color = vec4(light, 1.f) * texture(material.texture_diffuse1, vs_texture_coord);
}