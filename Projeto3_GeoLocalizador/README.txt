##### OBSERVAÇÕES #####

1) No arquivo não foi compactado junto os arquivos "Blocks_Aleatorios.csv" e "Location_Aleatorios.csv", devido ao tamanho. Eles são necessários para o funcionamento do programa. Para fazê-lo deve-se ter os arquivos do GeoLite Blocks e Locations e rodar os comandos "sort -R GeoLiteCity-Blocks.csv > Blocks_Aleatorios.csv" e "sort -R GeoLiteCity-Location.csv > Location_Aleatorios.csv";

2) As funções pesquisa por cidade e estado não foram implementadas por completo. O motivo acredito ser porque tem locais onde estas informações estejam em branco no arquivo GeoLite e, apesar de tentarmos utilizar o metodo size da classe string, mesmo assim não funciona.

3) A funcao pesquisa por cidade funcionou em nossos testes pesquisando por pais BR e cidade Rio De Janeiro, porém no teste seguinte, colocando Sao Paulo, nao rodou. Por conta disso, deixamos como nao implementada.
