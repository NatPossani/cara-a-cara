# Script para adicionar MinGW ao PATH do Windows
# Execute como Administrador para PATH do sistema, ou normalmente para PATH do usuario

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Adicionar MinGW ao PATH do Windows" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Caminho do MinGW
$mingwPath = "C:\msys64\mingw64\bin"

# Verificar se o caminho existe
if (-not (Test-Path $mingwPath)) {
    Write-Host "ERRO: Caminho nao encontrado: $mingwPath" -ForegroundColor Red
    Write-Host ""
    Write-Host "Verifique se o MSYS2 esta instalado corretamente." -ForegroundColor Yellow
    Write-Host ""
    pause
    exit 1
}

Write-Host "OK: Caminho encontrado: $mingwPath" -ForegroundColor Green
Write-Host ""

# Verificar se ja esta no PATH
$currentUserPath = [Environment]::GetEnvironmentVariable("Path", "User")
$currentSystemPath = [Environment]::GetEnvironmentVariable("Path", "Machine")

$alreadyInUserPath = $currentUserPath -like "*$mingwPath*"
$alreadyInSystemPath = $currentSystemPath -like "*$mingwPath*"

if ($alreadyInUserPath -or $alreadyInSystemPath) {
    Write-Host "O caminho ja esta no PATH!" -ForegroundColor Yellow
    if ($alreadyInUserPath) {
        Write-Host "   - Encontrado no PATH do usuario" -ForegroundColor Green
    }
    if ($alreadyInSystemPath) {
        Write-Host "   - Encontrado no PATH do sistema" -ForegroundColor Green
    }
    Write-Host ""
    Write-Host "Se o GCC ainda nao funciona, feche e reabra o terminal." -ForegroundColor Yellow
    Write-Host ""
    pause
    exit 0
}

# Adicionar ao PATH do usuario (mais simples, nao precisa de admin)
Write-Host "Adicionando ao PATH do usuario..." -ForegroundColor Yellow

$newPath = $currentUserPath
if ($newPath -and -not $newPath.EndsWith(";")) {
    $newPath += ";"
}
$newPath += $mingwPath

try {
    [Environment]::SetEnvironmentVariable("Path", $newPath, "User")
    Write-Host "SUCCESS: Caminho adicionado ao PATH do usuario!" -ForegroundColor Green
    Write-Host ""
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host "PROXIMOS PASSOS:" -ForegroundColor Cyan
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host "1. FECHE este terminal" -ForegroundColor Yellow
    Write-Host "2. Abra um NOVO terminal/PowerShell" -ForegroundColor Yellow
    Write-Host "3. Execute: gcc --version" -ForegroundColor Yellow
    Write-Host "4. Se funcionar, execute: compilar.bat" -ForegroundColor Yellow
    Write-Host ""
} catch {
    Write-Host "ERRO ao adicionar ao PATH: $_" -ForegroundColor Red
    pause
    exit 1
}

Write-Host "Pressione qualquer tecla para continuar..."
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
