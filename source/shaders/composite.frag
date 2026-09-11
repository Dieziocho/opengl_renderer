#version 430 core
out vec4 frag_color;

uniform sampler2DMS accumulate;
uniform sampler2DMS reveal;

//Comparation aproximate
const float EPSILON = 0.00001;

bool isApproximatelyEqual(float a, float b) {
    return abs(a - b) <= (abs(a) < abs(b) ? abs(b) : abs(a)) * EPSILON;
}

float max3(vec3 v) {
    return max(max(v.x, v.y), v.z);
}

void main() {
    ivec2 coordinates = ivec2(gl_FragCoord.xy);
    float revealage = texelFetch(reveal, coordinates, 0).r;

    //Save the blending and color texture fetch cost if there is not a transparent fragment
    if (isApproximatelyEqual(revealage, 1.0))
        discard;

    //Fragment color
    vec4 accumulation = texelFetch(accumulate, coordinates, 0);

    //Suppress overflow
    if (isinf(max3(abs(accumulation.rgb))))
        accumulation.rgb = vec3(accumulation.a);

    //Prevent floating point precision bug
    vec3 average_color = accumulation.rgb / max(accumulation.a, EPSILON);

    //Blend pixels
    frag_color = vec4(average_color, 1.0 - revealage);
}
