# 🔧 Como Adicionar MinGW ao PATH do Windows

## ✅ Boa Notícia!

O MSYS2 já está instalado em: `C:\msys64\mingw64\bin`

Agora precisamos adicionar essa pasta ao PATH do Windows para que você possa usar o `gcc` de qualquer lugar.

---

## 📋 Método 1: Usando Interface Gráfica (Recomendado para Iniciantes)

### Passo a Passo:

1. **Abra as Variáveis de Ambiente:**
   - Pressione `Win + R` (tecla Windows + R)
   - Digite: `sysdm.cpl`
   - Pressione Enter

2. **Ou alternativamente:**
   - Clique com botão direito em "Este Computador" (ou "Meu Computador")
   - Selecione "Propriedades"
   - Clique em "Configurações avançadas do sistema"
   - Clique no botão "Variáveis de Ambiente"

3. **Adicionar ao PATH do Usuário (Recomendado):**
   - Na seção "Variáveis do usuário", encontre a variável `Path`
   - Clique em "Editar"
   - Clique em "Novo"
   - Digite: `C:\msys64\mingw64\bin`
   - Clique em "OK"

4. **OU Adicionar ao PATH do Sistema (Para todos os usuários):**
   - Na seção "Variáveis do sistema", encontre a variável `Path`
   - Clique em "Editar"
   - Clique em "Novo"
   - Digite: `C:\msys64\mingw64\bin`
   - Clique em "OK"

5. **Aplicar as mudanças:**
   - Clique em "OK" em todas as janelas abertas
   - **IMPORTANTE:** Feche e reabra o terminal/PowerShell para as mudanças terem efeito

6. **Verificar se funcionou:**
   - Abra um NOVO terminal/PowerShell
   - Digite: `gcc --version`
   - Se mostrar a versão do GCC, está funcionando! ✅

---

## 🚀 Método 2: Usando PowerShell (Rápido - Requer Permissão de Administrador)

### Opção A: Script Automático

Execute o script `adicionar-ao-path.ps1` que criamos (veja abaixo).

### Opção B: Comando Manual

Abra o PowerShell como **Administrador** e execute:

```powershell
# Adicionar ao PATH do usuário atual
$currentPath = [Environment]::GetEnvironmentVariable("Path", "User")
$newPath = "C:\msys64\mingw64\bin"
if ($currentPath -notlike "*$newPath*") {
    [Environment]::SetEnvironmentVariable("Path", "$currentPath;$newPath", "User")
    Write-Host "✅ Caminho adicionado ao PATH do usuário!" -ForegroundColor Green
} else {
    Write-Host "ℹ️ Caminho já está no PATH" -ForegroundColor Yellow
}
```

**Nota:** Após executar, feche e reabra o terminal.

---

## 🔍 Verificar se Está Funcionando

1. **Feche o terminal atual**
2. **Abra um NOVO terminal/PowerShell**
3. **Execute:**
   ```bash
   gcc --version
   ```

Se mostrar algo como:
```
gcc (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 13.2.0
Copyright (C) 2023 Free Software Foundation, Inc.
```

**✅ Sucesso!** O GCC está funcionando.

---

## 🐛 Problemas Comuns

### Problema: "gcc não é reconhecido como comando"
**Solução:**
- Certifique-se de ter fechado e reaberto o terminal
- Verifique se o caminho está correto: `C:\msys64\mingw64\bin`
- Tente executar diretamente: `C:\msys64\mingw64\bin\gcc.exe --version`

### Problema: "Acesso negado" ao editar PATH
**Solução:**
- Use o PATH do usuário (não do sistema)
- Ou execute o PowerShell como Administrador

### Problema: MSYS2 não está em C:\msys64
**Solução:**
- Encontre onde o MSYS2 está instalado
- Procure pela pasta `mingw64\bin` dentro da instalação do MSYS2
- Use esse caminho completo no lugar de `C:\msys64\mingw64\bin`

---

## 📝 Próximos Passos

Após adicionar ao PATH:

1. **Feche e reabra o terminal**
2. **Execute:**
   ```bash
   compilar.bat
   ```
3. **Os executáveis serão gerados em:**
   - `build\server.exe`
   - `build\client.exe`

---

## 🎯 Resumo Rápido

1. Pressione `Win + R`
2. Digite: `sysdm.cpl` → Enter
3. Variáveis de Ambiente → Path → Editar → Novo
4. Adicione: `C:\msys64\mingw64\bin`
5. OK em todas as janelas
6. **Feche e reabra o terminal**
7. Teste: `gcc --version`

