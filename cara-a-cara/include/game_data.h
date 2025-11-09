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
        FEATURE_GENDER_FEM | FEATURE_HAIR_RED
    },
    {
        "Ben",
        "ʕ•ᴥ•ʔ",
        FEATURE_GENDER_MASC | FEATURE_BEARD
    },
    {
        "Chloe",
        "(づ｡◕‿◕｡)づ",
        FEATURE_GENDER_FEM | FEATURE_HAIR_BLONDE
    },
    {
        "Diego",
        "(ง •̀•́)ง",
        FEATURE_GENDER_MASC | FEATURE_HAIR_CURLY
    },
    {
        "Eva",
        "(∩^o^)⊃━☆",
        FEATURE_GENDER_FEM | FEATURE_HEADWEAR | FEATURE_PROF_EDUCACAO
    },
    {
        "Felix",
        "ヽ(⌐■-■)ノ♪",
        FEATURE_GENDER_MASC | FEATURE_HEADWEAR | FEATURE_UNIFORM
    },
    {
        "Gina",
        "/|\\ ^._.^ /|\\",
        FEATURE_GENDER_FEM | FEATURE_HEADWEAR | FEATURE_SCARF
    },
    {
        "Hugo",
        "(•̀ᴗ•́)و",
        FEATURE_GENDER_MASC | FEATURE_UNIFORM | FEATURE_PROF_SAUDE
    },
    {
        "Iris",
        "(◕‿◕✿)",
        FEATURE_GENDER_FEM | FEATURE_GLASSES | FEATURE_PROF_EDUCACAO
    },
    {
        "Kai",
        "(☞ﾟヮﾟ)☞",
        FEATURE_GENDER_MASC | FEATURE_HEADWEAR | FEATURE_UNIFORM | FEATURE_PROF_CULINARIA
    },
    {
        "Lara",
        "(＾▽＾)",
        FEATURE_GENDER_FEM | FEATURE_HAIR_RED | FEATURE_GLASSES
    },
    {
        "Milo",
        "(¬‿¬)",
        FEATURE_GENDER_MASC | FEATURE_HAIR_BLONDE
    },
    {
        "Nina",
        "ヘ(￣ー￣ヘ)",
        FEATURE_GENDER_FEM | FEATURE_HAIR_CURLY | FEATURE_SCARF
    },
    {
        "Oscar",
        "( ͡° ͜ʖ ͡°)",
        FEATURE_GENDER_MASC | FEATURE_BEARD | FEATURE_GLASSES
    },
    {
        "Pia",
        "(=^･ω･^=)",
        FEATURE_GENDER_FEM | FEATURE_UNIFORM
    },
    {
        "Quinn",
        "ƸӜƷ",
        FEATURE_GENDER_MASC | FEATURE_HEADWEAR
    },
    {
        "Rosa",
        "( •͡˘ •͡˘)ノð",
        FEATURE_GENDER_FEM | FEATURE_PROF_CULINARIA
    },
    {
        "Sergio",
        "ヽ(´▽`)/",
        FEATURE_GENDER_MASC | FEATURE_SCARF | FEATURE_HEADWEAR
    },
    {
        "Tara",
        "¯\(ツ)/¯",
        FEATURE_GENDER_FEM | FEATURE_GLASSES | FEATURE_UNIFORM
    },
    {
        "Uri",
        "(✿╹◡╹)",
        FEATURE_GENDER_MASC | FEATURE_HEADWEAR | FEATURE_BEARD
    },
    {
        "Vera",
        "(>^^)>",
        FEATURE_GENDER_FEM | FEATURE_PROF_SAUDE
    },
    {
        "Wes",
        "(╯°□°）╯︵ ┻━┻",
        FEATURE_GENDER_MASC | FEATURE_HAIR_RED
    },
    {
        "Xena",
        "(~˘▾˘)~",
        FEATURE_GENDER_FEM | FEATURE_HEADWEAR | FEATURE_HAIR_BLONDE
    },
    {
        "Yuri",
        "(ง°ل͜°)ง",
        FEATURE_GENDER_MASC | FEATURE_PROF_EDUCACAO
    },
    {
        "Zoe",
        "(~˘▾˘)~",
        FEATURE_GENDER_FEM | FEATURE_HAIR_CURLY | FEATURE_GLASSES
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

