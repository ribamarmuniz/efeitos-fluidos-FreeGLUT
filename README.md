# Efeitos Gráficos com FreeGLUT no Code::Blocks

Este repositório reúne um conjunto de efeitos gráficos desenvolvidos em C utilizando a biblioteca FreeGLUT, com o ambiente de desenvolvimento Code::Blocks.

O objetivo do projeto é explorar conceitos básicos de computação gráfica, animação, renderização em tempo real e manipulação de elementos visuais usando OpenGL e FreeGLUT.

## Sobre o Projeto

O projeto contém diferentes efeitos visuais criados para fins de estudo, experimentação e demonstração prática de recursos gráficos.

Entre os conceitos trabalhados estão os efietos:

* Água
* Vento
* Explosão
* Dispersão de fluidos
* Fumaça
* Tornado

## Tecnologias Utilizadas

* C/C++;
* OpenGL;
* FreeGLUT;
* Code::Blocks;
* GCC/MinGW.

## Ambiente de Desenvolvimento

O projeto foi desenvolvido no Code::Blocks, utilizando a biblioteca FreeGLUT para criação da janela gráfica e controle do loop principal de renderização.

Para executar corretamente, é necessário ter o FreeGLUT configurado no ambiente de desenvolvimento.

A estrutura pode variar conforme a organização local do projeto. O arquivo principal geralmente contém as funções de inicialização, renderização e atualização dos efeitos gráficos.

## Como Executar

### 1. Instale o Code::Blocks

Baixe e instale o Code::Blocks com compilador MinGW incluso.

### 2. Configure o FreeGLUT

Certifique-se de que os arquivos da biblioteca FreeGLUT estejam corretamente configurados no Code::Blocks.

Normalmente, são necessários:

```text
freeglut.h
freeglut.dll
libfreeglut.a
```

Esses arquivos devem ser adicionados aos diretórios corretos do compilador e do projeto.

### 3. Abra o Projeto

Abra o arquivo do projeto no Code::Blocks:

```text
nome-do-projeto.cbp
```

Esse arquivo carrega a configuração do projeto dentro do Code::Blocks.

### 4. Compile o Projeto

No Code::Blocks, clique em:

```text
Build → Build
```

Esse comando compila o código-fonte e gera o executável do projeto.

### 5. Execute

Depois da compilação, clique em:

```text
Build → Run
```

Esse comando executa o programa e abre a janela gráfica com os efeitos criados.

## Controles

Caso o projeto possua interação, os comandos podem ser adicionados abaixo:

```text
Teclas do teclado   - Controlam algum efeito específico
Botoes do mouse    - Interage com os elementos gráficos
```

## Objetivo Acadêmico

Este projeto foi desenvolvido com finalidade acadêmica e prática, servindo como exercício para compreender melhor os fundamentos da computação gráfica com OpenGL e FreeGLUT.

## Autor

Desenvolvido por **José Ribamar Cerqueira Muniz**.
