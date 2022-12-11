//Mientras mas cerca mas blanco
#version 330 core
out vec4 FragColor;

float near = 0.01;
float far = 100.0;

float linealizarProfundidad(float depth){
	
	float ndc = depth * 2.0 - 1.0;
	return (2.0 * near * far * (far + near - ndc * (far - near)));

}

void main()
{             
    //FragColor = vec4(1.0, 1.0, 1.0, 1.0);
	//Renderizado de la profundidad
	//FragColor = vec4(vec3(gl_FragCoord.z), 1.0);//nos regresa el valor de profundidad, no el eje z    0  -->  1 
	FragColor = vec4(vec3(linealizarProfundidad(gl_FragCoord.z)),1.0);
}

//Mientras mas cerca mas negro

//#version 330 core
//out vec4 FragColor;

//float near = 0.01;
//float far = 100.0;

//float linealizarProfundidad(float depth){
	
	//float ndc = depth * 2.0 - 1.0;
	//return (2.0 * near * far) / (far + near - ndc * (far - near));

//}

//void main()
//{             
    //FragColor = vec4(1.0, 1.0, 1.0, 1.0);
	//Renderizado de la profundidad
	//FragColor = vec4(vec3(gl_FragCoord.z), 1.0);//nos regresa el valor de profundidad, no el eje z    0  -->  1 
	//FragColor = vec4(vec3(linealizarProfundidad(gl_FragCoord.z)/far),1.0);
//}
