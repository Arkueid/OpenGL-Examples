#version 330 core

in vec2 TexCoords;  // 该顶点的材质坐标
in vec3 Normal;  // 该顶点处的法向量
in vec3 FragPos;  // 该顶点的位置坐标
out vec4 FragColor;

struct Material 
{
	sampler2D diffuse;  // 物体漫反射下的颜色
	vec3 specular;  // 物体镜面反射下的颜色
	float shineness;  // 物体镜面高光系数
};

struct Light
{
	vec3 position; 
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;	
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;  // 摄像机位置

void main() {
	// 环境光
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

	// 漫反射
	vec3 lightDir = normalize(vec3(-FragPos.x, FragPos.y, -FragPos.z));
	vec3 norm = normalize(Normal);
	float diff = max(0.0, dot(norm, lightDir));
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

	// 镜面光
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(0.0, dot(reflectDir, viewDir)), material.shineness);
	vec3 specular = light.specular * (spec * material.specular);

	FragColor = vec4(ambient + diffuse, 1.0);
}