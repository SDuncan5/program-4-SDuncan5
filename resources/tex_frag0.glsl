#version 330 core
uniform sampler2D Texture0;
uniform int flip;

in vec2 vTexCoord;
in vec3 fragNor;
in vec3 lightDir;

out vec4 Outcolor;

void main() {
    
    vec3 normal = normalize(fragNor);
    if (flip == 1){
        normal = normal * -1.0f;
    }
    vec3 light = normalize(lightDir);
    float dC = max(0, dot(normal, light));

    //vec4 texColor0 = texture(Texture0, vTexCoord); //texture color w/ tex coords
    //vec4 texColor0 = dC * texture(Texture0, vTexCoord) + vec4(MatAmb, 0.0);
    //vec4 texColorLighting = dC * texColor0 + texColor0;

    //vec4 texColor0 = texture(Texture0, vTexCoord); //texture color w/ tex coords
    vec4 texColor0 = (dC + 0.5) * texture(Texture0, vTexCoord);

    //vec3 phong = (ambient + diffuse) * objectColor + specular;
    //(dC + 0.3) * texture


  	//to set the out color as the texture color 
  	Outcolor = texColor0;
  	//Outcolor = texColorLighting;
  
  	//to set the outcolor as the texture coordinate (for debugging)
	//Outcolor = vec4(vTexCoord.s, vTexCoord.t, 0, 1);
}

