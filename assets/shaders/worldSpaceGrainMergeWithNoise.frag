uniform sampler2D texture;
uniform sampler2D noiseTexture;
uniform vec2 noiseTextureSize;

varying vec2 worldPos;

void main()
{
	vec4 texPixel = texture2D(texture, gl_TexCoord[0].xy);
	vec4 noisePixel = texture2D(noiseTexture, worldPos/noiseTextureSize);
    if(texPixel.a == 0)
    {
        gl_FragColor = (gl_Color * texPixel);
    }
    else
    {
        gl_FragColor = (gl_Color * texPixel) + noisePixel - 0.5;
    }   
	//gl_FragColor = noisePixel;
}