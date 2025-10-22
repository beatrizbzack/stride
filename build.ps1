$BISON = "win_bison"
$FLEX  = "win_flex"
$GCC   = "gcc"
$Y = "stride.y"
$L = "stride.l"
$TABC = "stride.tab.c"
$LEXC = "lex.yy.c"
$OUT = "stride_parser.exe"

Write-Host "1) bison -> $TABC"
& $BISON -d -o $TABC $Y
if ($LASTEXITCODE -ne 0) { Write-Error "bison falhou. Cole a saída aqui."; exit 1 }

Write-Host "2) flex -> $LEXC"
& $FLEX -o $LEXC $L
if ($LASTEXITCODE -ne 0) { Write-Error "flex falhou. Cole a saída aqui."; exit 1 }

Write-Host "3) gcc -> $OUT"
& $GCC -o $OUT $TABC $LEXC
if ($LASTEXITCODE -ne 0) {
    Write-Host "Compilação sem -lfl falhou; tentando com -lfl..."
    & $GCC -o $OUT $TABC $LEXC -lfl
    if ($LASTEXITCODE -ne 0) { Write-Error "Compilação falhou. Cole os erros aqui."; exit 1 }
}

Write-Host "Build concluído: .\$OUT"
