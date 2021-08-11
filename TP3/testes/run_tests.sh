echo "Testes Case List"

for i in {1..4..1}; do
  testname=$(printf "%d" $i)
  cd ..
  ./myparser testes/case_list/t$testname.cl > test1.results
  ./myparserCorrecto testes/case_list/t$testname.cl > test2.results
  cd testes

  if ! diff -qwB test1.results test2.results &>/dev/null; then
    echo "Teste $testname falhou"
  else
    echo "Teste $testname passou"
  fi
done
rm test1.results
rm test2.results

echo "Testes Class List"

for i in {1..3..1}; do
  testname=$(printf "%d" $i)
  cd ..
  ./myparser testes/class_list/t$testname.cl > test1.results
  ./myparserCorrecto testes/class_list/t$testname.cl > test2.results
  cd testes

  if ! diff -qwB test1.results test2.results &>/dev/null; then
    echo "Teste $testname falhou"
  else
    echo "Teste $testname passou"
  fi
done
rm test1.results
rm test2.results

echo "Testes Expression"

for i in {1..2..1}; do
  testname=$(printf "%d" $i)
  cd ..
  ./myparser testes/expression/t$testname.cl > test1.results
  ./myparserCorrecto testes/expression/t$testname.cl > test2.results
  cd testes

  if ! diff -qwB test1.results test2.results &>/dev/null; then
    echo "Teste $testname falhou"
  else
    echo "Teste $testname passou"
  fi
done
rm test1.results
rm test2.results

echo "Testes Expression Let"

for i in {1..6..1}; do
  testname=$(printf "%d" $i)
  cd ..
  ./myparser testes/expression_let/t$testname.cl > test1.results
  ./myparserCorrecto testes/expression_let/t$testname.cl > test2.results
  cd testes

  if ! diff -qwB test1.results test2.results &>/dev/null; then
    echo "Teste $testname falhou"
  else
    echo "Teste $testname passou"
  fi
done
rm test1.results
rm test2.results

echo "Testes Expression List Comma"

for i in {1..4..1}; do
  testname=$(printf "%d" $i)
  cd ..
  ./myparser testes/expression_list_comma/t$testname.cl > test1.results
  ./myparserCorrecto testes/expression_list_comma/t$testname.cl > test2.results
  cd testes

  if ! diff -qwB test1.results test2.results &>/dev/null; then
    echo "Teste $testname falhou"
  else
    echo "Teste $testname passou"
  fi
done
rm test1.results
rm test2.results

echo "Testes Expression List Semicolon"

for i in {1..3..1}; do
  testname=$(printf "%d" $i)
  cd ..
  ./myparser testes/expression_list_semic/t$testname.cl > test1.results
  ./myparserCorrecto testes/expression_list_semic/t$testname.cl > test2.results
  cd testes

  if ! diff -qwB test1.results test2.results &>/dev/null; then
    echo "Teste $testname falhou"
  else
    echo "Teste $testname passou"
  fi
done
rm test1.results
rm test2.results

echo "Testes Feature List"

for i in {1..4..1}; do
  testname=$(printf "%d" $i)
  cd ..
  ./myparser testes/feature_list/t$testname.cl > test1.results
  ./myparserCorrecto testes/feature_list/t$testname.cl > test2.results
  cd testes

  if ! diff -qwB test1.results test2.results &>/dev/null; then
    echo "Teste $testname falhou"
  else
    echo "Teste $testname passou"
  fi
done
rm test1.results
rm test2.results

echo "Testes Formal List"

for i in {1..4..1}; do
  testname=$(printf "%d" $i)
  cd ..
  ./myparser testes/formal_list/t$testname.cl > test1.results
  ./myparserCorrecto testes/formal_list/t$testname.cl > test2.results
  cd testes

  if ! diff -qwB test1.results test2.results &>/dev/null; then
    echo "Teste $testname falhou"
  else
    echo "Teste $testname passou"
  fi
done
rm test1.results
rm test2.results
