#version 330 core

out vec4 FragColor;

// 光照所需
in vec3 Normal;  // 该顶点处的法向量
in vec3 FragPos;  // 该顶点的位置坐标
struct Material 
{
	vec3 diffuse;  // 物体漫反射下的颜色
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
	vec3 ambient = light.ambient * material.diffuse;

	// 漫反射
	vec3 lightDir = normalize(vec3(light.position - FragPos));
	vec3 norm = normalize(Normal);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * material.diffuse;

	// 镜面光
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = normalize(reflect(-lightDir, norm));
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shineness);
	vec3 specular = light.specular * spec * material.specular;

	float d = length(light.position - FragPos);

	float att = 1.0 / (1.0 + 0.07 * d + 0.017 * d * d);

	vec3 result = ambient + diffuse + specular;
	result = result * att;
	FragColor = vec4(result, 1.0);
}