#ifndef GAME_DATA_H
#define GAME_DATA_H

#define MAX_CHARS 25

typedef struct {
    const char* nome;
    const char* emoji;
} PersonagemDados;

typedef struct {
    const char* pergunta;
    const char* dica;
} SugestaoPergunta;

static const PersonagemDados PERSONAGENS_DADOS[MAX_CHARS] = {
    {"Aline",   "☜(⌒▽⌒)☞"},
    {"Bruno",   "ʕ•ᴥ•ʔ"},
    {"Camila",  "(づ｡◕‿◕｡)づ"},
    {"Diego",   "(ง •̀•́)ง"},
    {"Eduarda", "(∩^o^)⊃━☆"},
    {"Felipe",  "ヽ(⌐■-■)ノ♪"},
    {"Gabriela","/|\\ ^._.^ /|\\"},
    {"Henrique","(•̀ᴗ•́)و"},
    {"Isabela", "(◕‿◕✿)"},
    {"Kaio",    "(☞ﾟヮﾟ)☞"},
    {"Larissa", "(＾▽＾)"},
    {"Miguel",  "(¬‿¬)"},
    {"Natalia", "ヘ(￣ー￣ヘ)"},
    {"Otavio",  "( ͡° ͜ʖ ͡°)"},
    {"Paula",   "(=^･ω･^=)"},
    {"Queila",  "ƸӜƷ"},
    {"Rafaela", "( •͡˘ •͡˘)ノð"},
    {"Samuel",  "ヽ(´▽`)/"},
    {"Talita",  "¯\\(ツ)/¯"},
    {"Ulisses", "(✿╹◡╹)"},
    {"Valeria", "(>^^)>"},
    {"Wesley",  "(╯°□°）╯︵ ┻━┻"},
    {"Ximena",  "(~˘▾˘)~"},
    {"Yago",    "(ง°ل͜°)ง"},
    {"Zilda",   "(~˘▾˘)~"}
};

static const SugestaoPergunta SUGESTOES_PERGUNTAS[] = {
    {"Seu personagem é mulher?", "Divide o tabuleiro quase ao meio logo no começo."},
    {"Seu personagem é homem?", "Pergunta complementar caso prefira confirmar o gênero masculino."},
    {"Seu personagem usa algo na cabeça (chapéu, lenço, capelo)?", "Filtra rapidamente quem usa acessórios na cabeça."},
    {"Seu personagem usa óculos?", "Útil para identificar personagens estudiosos."},
    {"Seu personagem tem barba?", "Perfeito para separar o Bruno do restante."},
    {"O cabelo do seu personagem é ruivo?", "Excelente quando restam personagens sem acessórios."},
    {"O cabelo do seu personagem é loiro?", "Boa para diferenciar tons de cabelo claros."},
    {"O cabelo do seu personagem é cacheado?", "Ajuda a isolar estilos de cabelo específicos."},
    {"Seu personagem veste algum uniforme de trabalho?", "Destaca pilotos, médicos e chefs de cozinha."},
    {"Seu personagem trabalha com educação?", "Identifica figuras acadêmicas como Eduarda e Isabela."},
    {"Seu personagem trabalha na área da saúde?", "Aponta diretamente para o médico do grupo."},
    {"Seu personagem trabalha com culinária?", "Checa se o chef Kaio é o personagem escolhido."}
};

static const int NUM_SUGESTOES_PERGUNTAS =
    (int)(sizeof(SUGESTOES_PERGUNTAS) / sizeof(SUGESTOES_PERGUNTAS[0]));

#endif // GAME_DATA_H

