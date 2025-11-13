#ifndef GAME_DATA_H
#define GAME_DATA_H

#define MAX_CHARS 25

// Atributos possíveis dos personagens (bitmask)
#define FEATURE_GENDER_MASC      (1u << 0)
#define FEATURE_GENDER_FEM       (1u << 1)
#define FEATURE_HEADWEAR         (1u << 2)
#define FEATURE_GLASSES          (1u << 3)
#define FEATURE_BEARD            (1u << 4)
#define FEATURE_HAIR_RED         (1u << 5)
#define FEATURE_HAIR_BLONDE      (1u << 6)
#define FEATURE_HAIR_CURLY       (1u << 7)
#define FEATURE_UNIFORM          (1u << 8)
#define FEATURE_SCARF            (1u << 9)
#define FEATURE_PROF_SAUDE       (1u << 10)
#define FEATURE_PROF_EDUCACAO    (1u << 11)
#define FEATURE_PROF_CULINARIA   (1u << 12)

typedef struct {
    const char* nome;
    const char* emoji;
    unsigned int atributos;
    const char* resumo;
} PersonagemDados;

typedef struct {
    const char* pergunta;
    unsigned int atributo_mask;
    const char* dica;
} SugestaoPergunta;

static const PersonagemDados PERSONAGENS_DADOS[MAX_CHARS] = {
    {
        "Ava",
        "☜(⌒▽⌒)☞",
        FEATURE_GENDER_FEM | FEATURE_HAIR_RED,
        "Cabelos ruivos soltos e sorriso confiante."
    },
    {
        "Ben",
        "ʕ•ᴥ•ʔ",
        FEATURE_GENDER_MASC | FEATURE_BEARD,
        "Barba cheia e casaco casual."
    },
    {
        "Chloe",
        "(づ｡◕‿◕｡)づ",
        FEATURE_GENDER_FEM | FEATURE_HAIR_BLONDE,
        "Cabelos loiros e visual descontraido."
    },
    {
        "Diego",
        "(ง •̀•́)ง",
        FEATURE_GENDER_MASC | FEATURE_HAIR_CURLY,
        "Cabelos cacheados e expressao animada."
    },
    {
        "Eva",
        "(∩^o^)⊃━☆",
        FEATURE_GENDER_FEM | FEATURE_HEADWEAR | FEATURE_PROF_EDUCACAO,
        "Capelo de formatura e traje academico."
    },
    {
        "Felix",
        "ヽ(⌐■-■)ノ♪",
        FEATURE_GENDER_MASC | FEATURE_HEADWEAR | FEATURE_UNIFORM,
        "Uniforme impecavel de piloto e quepe."
    },
    {
        "Gina",
        "/|\\ ^._.^ /|\\",
        FEATURE_GENDER_FEM | FEATURE_HEADWEAR | FEATURE_SCARF,
        "Lenco colorido envolvendo o cabelo."
    },
    {
        "Hugo",
        "(•̀ᴗ•́)و",
        FEATURE_GENDER_MASC | FEATURE_UNIFORM | FEATURE_PROF_SAUDE,
        "Jaleco branco e estetoscopio."
    },
    {
        "Iris",
        "(◕‿◕✿)",
        FEATURE_GENDER_FEM | FEATURE_GLASSES | FEATURE_PROF_EDUCACAO,
        "Oculos e blazer de professora."
    },
    {
        "Kai",
        "(☞ﾟヮﾟ)☞",
        FEATURE_GENDER_MASC | FEATURE_HEADWEAR | FEATURE_UNIFORM | FEATURE_PROF_CULINARIA,
        "Chapeu de chef e avental."
    },
    {
        "Lara",
        "(＾▽＾)",
        FEATURE_GENDER_FEM | FEATURE_HAIR_RED | FEATURE_GLASSES,
        "Oculos modernos e cabelos ruivos presos."
    },
    {
        "Milo",
        "(¬‿¬)",
        FEATURE_GENDER_MASC | FEATURE_HAIR_BLONDE,
        "Topete loiro desarrumado e jaqueta de couro."
    },
    {
        "Nina",
        "ヘ(￣ー￣ヘ)",
        FEATURE_GENDER_FEM | FEATURE_HAIR_CURLY | FEATURE_SCARF,
        "Cachos volumosos e lenco estampado."
    },
    {
        "Oscar",
        "( ͡° ͜ʖ ͡°)",
        FEATURE_GENDER_MASC | FEATURE_BEARD | FEATURE_GLASSES,
        "Oculos escuros, barba aparada e postura confiante."
    },
    {
        "Pia",
        "(=^･ω･^=)",
        FEATURE_GENDER_FEM | FEATURE_UNIFORM,
        "Uniforme elegante e prancheta em maos."
    },
    {
        "Quinn",
        "ƸӜƷ",
        FEATURE_GENDER_MASC | FEATURE_HEADWEAR,
        "Bone virado para tras e sorriso ousado."
    },
    {
        "Rosa",
        "( •͡˘ •͡˘)ノð",
        FEATURE_GENDER_FEM | FEATURE_PROF_CULINARIA,
        "Avental colorido e olhar alegre."
    },
    {
        "Sergio",
        "ヽ(´▽`)/",
        FEATURE_GENDER_MASC | FEATURE_SCARF | FEATURE_HEADWEAR,
        "Cachecol grosso, boina e olhar atento."
    },
    {
        "Tara",
        "¯\(ツ)/¯",
        FEATURE_GENDER_FEM | FEATURE_GLASSES | FEATURE_UNIFORM,
        "Blazer escolar e oculos redondos."
    },
    {
        "Uri",
        "(✿╹◡╹)",
        FEATURE_GENDER_MASC | FEATURE_HEADWEAR | FEATURE_BEARD,
        "Barba curta, boina escura e semblante serio."
    },
    {
        "Vera",
        "(>^^)>",
        FEATURE_GENDER_FEM | FEATURE_PROF_SAUDE,
        "Jaleco de enfermagem e sorriso gentil."
    },
    {
        "Wes",
        "(╯°□°）╯︵ ┻━┻",
        FEATURE_GENDER_MASC | FEATURE_HAIR_RED,
        "Cabelo ruivo curto e camiseta esportiva."
    },
    {
        "Xena",
        "(~˘▾˘)~",
        FEATURE_GENDER_FEM | FEATURE_HEADWEAR | FEATURE_HAIR_BLONDE,
        "Chapeu vistoso, cabelos loiros e olhar decidido."
    },
    {
        "Yuri",
        "(ง°ل͜°)ง",
        FEATURE_GENDER_MASC | FEATURE_PROF_EDUCACAO,
        "Blazer academico e pilha de livros."
    },
    {
        "Zoe",
        "(~˘▾˘)~",
        FEATURE_GENDER_FEM | FEATURE_HAIR_CURLY | FEATURE_GLASSES,
        "Cachos definidos, oculos quadrados e energia contagiante."
    }
};

static const SugestaoPergunta SUGESTOES_PERGUNTAS[] = {
    {
        "Seu personagem é mulher?",
        FEATURE_GENDER_FEM,
        "Divide o tabuleiro quase ao meio logo no começo."
    },
    {
        "Seu personagem é homem?",
        FEATURE_GENDER_MASC,
        "Pergunta complementar caso prefira confirmar o gênero masculino."
    },
    {
        "Seu personagem usa algo na cabeça (chapéu, lenço, capelo)?",
        FEATURE_HEADWEAR | FEATURE_SCARF,
        "Filtra rapidamente quem usa acessórios na cabeça."
    },
    {
        "Seu personagem usa óculos?",
        FEATURE_GLASSES,
        "Útil para identificar personagens estudiosos."
    },
    {
        "Seu personagem tem barba?",
        FEATURE_BEARD,
        "Perfeito para separar o Ben do restante."
    },
    {
        "O cabelo do seu personagem é ruivo?",
        FEATURE_HAIR_RED,
        "Excelente quando restam personagens sem acessórios."
    },
    {
        "O cabelo do seu personagem é loiro?",
        FEATURE_HAIR_BLONDE,
        "Boa para diferenciar tons de cabelo claros."
    },
    {
        "O cabelo do seu personagem é cacheado?",
        FEATURE_HAIR_CURLY,
        "Ajuda a isolar estilos de cabelo específicos."
    },
    {
        "Seu personagem veste algum uniforme de trabalho?",
        FEATURE_UNIFORM,
        "Destaca pilotos, médicos e chefs de cozinha."
    },
    {
        "Seu personagem trabalha com educação?",
        FEATURE_PROF_EDUCACAO,
        "Identifica figuras acadêmicas como Eva e Iris."
    },
    {
        "Seu personagem trabalha na área da saúde?",
        FEATURE_PROF_SAUDE,
        "Aponta diretamente para o médico do grupo."
    },
    {
        "Seu personagem trabalha com culinária?",
        FEATURE_PROF_CULINARIA,
        "Checa se o chef Kai é o personagem escolhido."
    }
};

static const int NUM_SUGESTOES_PERGUNTAS =
    (int)(sizeof(SUGESTOES_PERGUNTAS) / sizeof(SUGESTOES_PERGUNTAS[0]));

#endif // GAME_DATA_H

