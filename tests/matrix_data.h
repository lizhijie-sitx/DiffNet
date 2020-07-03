const double Ris[4][25] = {
    {
        1.09208268253, 1.00905258937, 1.04966627805, 0.668601688939, 0.773141346251,
        1.00905258937, 1.5923093812, 1.17543480844, 1.01562079503, 1.55723062095,
        1.04966627805, 1.17543480844, 1.2611182198, 0.863830701308, 1.21015200653,
        0.668601688939, 1.01562079503, 0.863830701308, 1.33729378071, 1.35913900968,
        0.773141346251, 1.55723062095, 1.21015200653, 1.35913900968, 2.06580355064
    },
    {
        2.77946745518, 1.89261843459, 1.35691186805, 1.17327321213, 1.76781280265,
        1.89261843459, 1.72892994968, 1.40900123489, 0.624951978972, 1.54358134627,
        1.35691186805, 1.40900123489, 1.39135859642, 0.514229933051, 1.41927128361,
        1.17327321213, 0.624951978972, 0.514229933051, 0.728674099717, 0.725443065181,
        1.76781280265, 1.54358134627, 1.41927128361, 0.725443065181, 2.18365879966
    },
    {
        1.16845586752, 0.773075829775, 0.662043062815, 0.708677141824, 0.821111719711,
        0.773075829775, 0.579361776974, 0.506073041414, 0.620885814523, 0.614100705089,
        0.662043062815, 0.506073041414, 1.02231469032, 1.26885971529, 0.84073690913,
        0.708677141824, 0.620885814523, 1.26885971529, 1.84613402499, 0.936350569704,
        0.821111719711, 0.614100705089, 0.84073690913, 0.936350569704, 0.986500936167
    },
    {
        1.62320797241, 0.788862601358, 0.613978433666, 1.53424652459, 1.06941488189,
        0.788862601358, 0.618143215747, 0.325642880172, 0.948501968875, 0.846933964691,
        0.613978433666, 0.325642880172, 0.264490721986, 0.595269696944, 0.462890538134,
        1.53424652459, 0.948501968875, 0.595269696944, 1.63025389391, 1.27474317318,
        1.06941488189, 0.846933964691, 0.462890538134, 1.27474317318, 1.46093326711
    }
};

const double ddR2[100] = {
    12.918177156, 1.64585275415, 3.30046174052, 2.97850671417, 5.82014210945, 0.331543744591, 1.19310874871, 3.75827964695, 1.02800147353, 4.67973393832,
    1.64585275415, 1.44796799154, 1.28064603742, 0.455956556829, 0.433652648185, 0.271113765518, 0.387216110794, 0.12599506436, 0.502545154777, 0.771882436423,
    3.30046174052, 1.28064603742, 3.37370606805, 1.78198753287, 0.547431637749, 0.676829676251, 0.942186893976, 0.297835806965, 0.868653378031, 1.66793440944,
    2.97850671417, 0.455956556829, 1.78198753287, 5.12752034949, 1.65566791242, 0.825530226182, 2.78308604581, 1.1132241717, 1.18342630931, 1.94784306038,
    5.82014210945, 0.433652648185, 0.547431637749, 1.65566791242, 6.24240904018, 0.367066477036, 1.66219823671, 3.72908467748, 1.04140917184, 2.70720575495,
    0.331543744591, 0.271113765518, 0.676829676251, 0.825530226182, 0.367066477036, 0.745009452729, 0.637945369978, 0.27789793619, 0.16772162073, 0.579942818008,
    1.19310874871, 0.387216110794, 0.942186893976, 2.78308604581, 1.66219823671, 0.637945369978, 3.79027310352, 1.55225493234, 1.78174676893, 2.0802506526,
    3.75827964695, 0.12599506436, 0.297835806965, 1.1132241717, 3.72908467748, 0.27789793619, 1.55225493234, 4.6413805762, 1.09739124406, 2.97498689374,
    1.02800147353, 0.502545154777, 0.868653378031, 1.18342630931, 1.04140917184, 0.16772162073, 1.78174676893, 1.09739124406, 2.55502470149, 1.67459150479,
    4.67973393832, 0.771882436423, 1.66793440944, 1.94784306038, 2.70720575495, 0.579942818008, 2.0802506526, 2.97498689374, 1.67459150479, 8.38525919637
};