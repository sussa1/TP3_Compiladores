TMP_OUT1=test1.results
TMP_OUT2=test2.results

echo "Testes Case List"

for i in {1..4..1}; do
  testname=$(printf "%d" $i)
  ./myparser testes/case_list/t$testname.cl > $TMP1_OUT
  ./myparserCorrecto testes/case_list/t$testname.cl > $TMP2_OUT

  if ! diff -qwB $TMP_OUT1 $TMP_OUT2 &>/dev/null; then
    echo "Teste $testname falhou"
  else
    echo "Teste $testname passou"
  fi
done
rm $TMP_OUT1
rm $TMP_OUT2

echo "Testes Class List"

for i in {1..3..1}; do
  testname=$(printf "%d" $i)
  ./myparser testes/class_list/t$testname.cl > $TMP1_OUT
  ./myparserCorrecto testes/class_list/t$testname.cl > $TMP2_OUT

  if ! diff -qwB $TMP_OUT1 $TMP_OUT2 &>/dev/null; then
    echo "Teste $testname falhou"
  else
    echo "Teste $testname passou"
  fi
done
rm $TMP_OUT1
rm $TMP_OUT2

echo "Testes Expression"

for i in {1..2..1}; do
  testname=$(printf "%d" $i)
  ./myparser testes/expression/t$testname.cl > $TMP1_OUT
  ./myparserCorrecto testes/expression/t$testname.cl > $TMP2_OUT

  if ! diff -qwB $TMP_OUT1 $TMP_OUT2 &>/dev/null; then
    echo "Teste $testname falhou"
  else
    echo "Teste $testname passou"
  fi
done
rm $TMP_OUT1
rm $TMP_OUT2

echo "Testes Expression Let"

for i in {1..6..1}; do
  testname=$(printf "%d" $i)
  ./myparser testes/expression_let/t$testname.cl > $TMP1_OUT
  ./myparserCorrecto testes/expression_let/t$testname.cl > $TMP2_OUT

  if ! diff -qwB $TMP_OUT1 $TMP_OUT2 &>/dev/null; then
    echo "Teste $testname falhou"
  else
    echo "Teste $testname passou"
  fi
done
rm $TMP_OUT1
rm $TMP_OUT2

echo "Testes Expression List Comma"

for i in {1..4..1}; do
  testname=$(printf "%d" $i)
  ./myparser testes/expression_list_comma/t$testname.cl > $TMP1_OUT
  ./myparserCorrecto testes/expression_list_comma/t$testname.cl > $TMP2_OUT

  if ! diff -qwB $TMP_OUT1 $TMP_OUT2 &>/dev/null; then
    echo "Teste $testname falhou"
  else
    echo "Teste $testname passou"
  fi
done
rm $TMP_OUT1
rm $TMP_OUT2

echo "Testes Expression List Semicolon"

for i in {1..3..1}; do
  testname=$(printf "%d" $i)
  ./myparser testes/expression_list_semic/t$testname.cl > $TMP1_OUT
  ./myparserCorrecto testes/expression_list_semic/t$testname.cl > $TMP2_OUT

  if ! diff -qwB $TMP_OUT1 $TMP_OUT2 &>/dev/null; then
    echo "Teste $testname falhou"
  else
    echo "Teste $testname passou"
  fi
done
rm $TMP_OUT1
rm $TMP_OUT2

echo "Testes Feature List"

for i in {1..4..1}; do
  testname=$(printf "%d" $i)
  ./myparser testes/feature_list/t$testname.cl > $TMP1_OUT
  ./myparserCorrecto testes/feature_list/t$testname.cl > $TMP2_OUT

  if ! diff -qwB $TMP_OUT1 $TMP_OUT2 &>/dev/null; then
    echo "Teste $testname falhou"
  else
    echo "Teste $testname passou"
  fi
done
rm $TMP_OUT1
rm $TMP_OUT2

echo "Testes Formal List"

for i in {1..4..1}; do
  testname=$(printf "%d" $i)
  ./myparser testes/formal_list/t$testname.cl > $TMP1_OUT
  ./myparserCorrecto testes/formal_list/t$testname.cl > $TMP2_OUT

  if ! diff -qwB $TMP_OUT1 $TMP_OUT2 &>/dev/null; then
    echo "Teste $testname falhou"
  else
    echo "Teste $testname passou"
  fi
done
rm $TMP_OUT1
rm $TMP_OUT2
