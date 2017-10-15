
#if EARLY_FRAGMENT_TESTS && !ALPHA_TEST
layout(early_fragment_tests) in;
#endif

/********************Entrant********************/
in vec2 vTexCoord;
in vec2 vFactors; // x=destruction y=ore

/********************Sortant********************/
out vec4 RenderTarget0;

/********************Uniformes********************/
uniform vec2 InvTargetSize;

uniform sampler2D MaterialDiffuseMap; //rockTexture
uniform sampler2D MaterialNormalMap; //brokenRockTexture
uniform sampler2D MaterialSpecularMap; //oreTexture

/********************Fonctions********************/
void main()
{
	vec4 fragmentColor = vec4(1.0f);

#if AUTO_TEXCOORDS
	vec2 texCoord = gl_FragCoord.xy * InvTargetSize;
#else
	vec2 texCoord = vTexCoord;
#endif

#if DIFFUSE_MAPPING && NORMAL_MAPPING && SPECULAR_MAPPING
	float oreFactor = vFactors.x;
	float destructionFactor = max(vFactors.y - oreFactor, 0);
	float rockFactor = 1-(oreFactor + destructionFactor);
	vec4 blend = texture(MaterialDiffuseMap, texCoord)*rockFactor + texture(MaterialNormalMap, texCoord)*destructionFactor + texture(MaterialSpecularMap, texCoord)*oreFactor;

	fragmentColor *= blend;


#endif

	RenderTarget0 = fragmentColor;
}