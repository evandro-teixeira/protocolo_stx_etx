# Protocolo de comunicação STX ETX

## Descrição

O padrão de comunicação STX/ETX que se destaca com as seguintes característica:<br> 
-> Master-Slave,<br> 
-> comunicação serial ponto a ponto,<br> 
-> framing binário assíncrono,<br> 
-> byte-oriented, segmentação por delimitadores de dados e comprimento de frames variável.<br> 
Tratando-se de um protocolo flexível e fácil de implementação dentro do padrão de sistema de segurança presentes no mercado.<p>
  
## Aplicação de Demonstração 

A aplicação de demonstração consiste por parte do Master enviar comandos para Slave.
A aplicação desenvolvida para o Master, é uma aplicação para desktop. Que por sua vez interage com o usuario para montar a mensagem a ser enviada para o Slave.
A aplicação desenvolvida para o Slave, é um firmware para STM32F0DISCOVERY, trata-se de kit de desenvolvimento para o microcontrolador STM32F0 que por sua vez é baseado arquitetura ARM Cortex-MO.
Os comandos proposto para a aplicação limita-se em acionar os LED’s presente na STM32F0DISCOVERY e ler o status do USER Button. 

### Master

O Master por sua vez tem como funcionalidade interagir com o usuário a fim de construir a mensagem a ser enviado para o Slave, obedecendo a regra demonstrada na figura a abaixo.

![Frame MSG](https://github.com/evandro-teixeira/protocolo_stx_etx/blob/master/Doc/Frame_MSG_STX_ETX.png)

O frame da mensagem é composta por 03 partes, o cabeçalho, conteúdo da mensagem e byte de encerramento (Fim de Mensagem).
O cabeçalho basicamente é utilizado para sincronismo, informando ao slave que se tratra de mensagem válida.
O conteúdo da mensagem, é mensagem propriamente dita a ser enviado ao slave. Para essa aplicação de demonstração a mensagem é composta pelo os seguintes itens; número de bytes que contém o MSG Data, o número do comando, MSG Data que é utilizado para informações variáveis, limitado a 04 bytes e por fim o Checksum, que por sua vez é calculado em cima do conteúdo da MSG. 
O byte de encerramento é byte que sinaliza o fim da mensagem. 
        
A interação com o usuário é bem intuitiva conforme pode ser vista nas figuras abaixo:

![Set LED Blue](https://github.com/evandro-teixeira/protocolo_stx_etx/blob/master/Doc/Captura%20de%20tela%20Set%20LED%20Blue.png)
Comando para acionar o LED Blue

![Set LED Green](https://github.com/evandro-teixeira/protocolo_stx_etx/blob/master/Doc/Captura%20de%20tela%20de%20Set%20LED%20Green.png)
Comando para acionar o LED Green

![Get Status User Bottun](https://github.com/evandro-teixeira/protocolo_stx_etx/blob/master/Doc/Captura%20de%20tela%20Get%20Status%20User%20Button.png)
Comando para ler o status do User Button 

### Slave

A aplicação desenvolvida para o slave consiste em ler os dados recebidos pelo barramento serial (UART), reconstruir a mensagem e tomar as ações de acordo com as mensagem recebidas. 
A aplicação foi desenvolvida utilizado o FreeRTOS. O algoritmo foi dividido em 03 tarefas (task / thread). Como pode ser vista na figura abaixo.

![Fluxograma da aplicação de demostração](https://github.com/evandro-teixeira/protocolo_stx_etx/blob/master/Doc/Fluxograma_App_STX_ETX.png)
