#version 330 core
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 objectColor;  // 物体本身的颜色
uniform vec3 lightColor;  // 光的颜色
uniform vec3 lightPos;  // 光源位置
uniform vec3 viewPos;  // 相机位置

void main() {
	float ambientStrength = 0.1;  // 环境光强，无论物体在何处，都有颜色
	vec3 ambient = ambientStrength * lightColor;  // 环境光
	vec3 norm = normalize(Normal);  // 归一化
	
	vec3 lightDir = normalize(lightPos - FragPos);  // 光线到该点的方向向量
	float diff = max(dot(norm, lightDir), 0.0);  // 点乘得到cos值，两个向量的模都为1，cos值越大说明入射角越小，反射的光越多，光照越强
	vec3 diffuse = diff * lightColor;  // 漫反射光线

	// 镜面光照
	// 镜面反射强度
	float specularStrength = 0.5;
	// 视线
	vec3 viewDir = normalize(viewPos - FragPos);
	// 反射光线
	vec3 reflectDir = reflect(-lightDir, norm);
	// 视线与反射光线的夹角的32次方
	float spec = pow(dot(viewDir, reflectDir), 32);
	// 反射光线
	vec3 specular = specularStrength * spec * lightColor;
	vec3 result = (ambient + diffuse + specular) * objectColor;  
	FragColor = vec4(result, 1.0f);
}