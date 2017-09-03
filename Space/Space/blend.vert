/********************Entrant********************/
in mat4 InstanceData0;

in vec3 VertexPosition;
in vec2 VertexTexCoord;
in vec2 VertexUserdata0;

/********************Sortant********************/
out vec2 vTexCoord;
out vec2 vFactors;

/********************Uniformes********************/
uniform float VertexDepth;
uniform mat4 ViewMatrix;
uniform mat4 ViewProjMatrix;
uniform mat4 WorldViewProjMatrix;

/********************Fonctions********************/
void main()
{
	vec2 texCoords;

#if FLAG_INSTANCING
	#if TRANSFORM
	gl_Position = ViewProjMatrix * InstanceData0 * vec4(VertexPosition, 1.0);
	#else
		#if UNIFORM_VERTEX_DEPTH
	gl_Position = InstanceData0 * vec4(VertexPosition.xy, VertexDepth, 1.0);
		#else
	gl_Position = InstanceData0 * vec4(VertexPosition, 1.0);
		#endif
	#endif
#else
	#if TRANSFORM
	gl_Position = WorldViewProjMatrix * vec4(VertexPosition, 1.0);
	#else
		#if UNIFORM_VERTEX_DEPTH
	gl_Position = vec4(VertexPosition.xy, VertexDepth, 1.0);
		#else
	gl_Position = vec4(VertexPosition, 1.0);
		#endif
	#endif
#endif

	texCoords = VertexTexCoord;

#if TEXTURE_MAPPING
	vTexCoord = vec2(texCoords);
#endif

	vFactors = VertexUserdata0;
}