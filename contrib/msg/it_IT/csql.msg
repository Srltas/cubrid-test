
$
$  Copyright 2008 Search Solution Corporation
$  Copyright 2016 CUBRID Corporation
$ 
$   Licensed under the Apache License, Version 2.0 (the "License");
$   you may not use this file except in compliance with the License.
$   You may obtain a copy of the License at
$ 
$       http://www.apache.org/licenses/LICENSE-2.0
$ 
$   Unless required by applicable law or agreed to in writing, software
$   distributed under the License is distributed on an "AS IS" BASIS,
$   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
$   See the License for the specific language governing permissions and
$   limitations under the License.
$ 
$ 
$

$
$ $Id$
$
$ csql.msg
$

$set 1 MSGCAT_CSQL_SET_CSQL
40 \
SQL utility interattiva , versione %s\n\
uso : %s [OPZIONE] nome-database\n\
\n\
opzioni valide:\n\
  -S, --SA-mode                esecuzione modalit? standalone\n\
  -C, --CS-mode                esecuzione modalit? client-server\n\
  -u, --user=ARG               user name alternativo\n\
  -p, --pasword=ARG            password , digitare "" per niente\n\
  -e, --error-continue         non uscire su errore\n\
  -i, --input-file=ARG         nome file input\n\
  -o, --output-file=ARG        nome file output\n\
  -s, --single-line            esecuzione per singola riga\n\
  -c, --command=ARG            comando CSQL\n\
  -l, --line-output            mostra ogni valore in una linea\n\
  -r, --read-only              modo read-only\n\
      --no-auto-commit         disabilita esecuzione auto commit\n\
      --no-pager               non usare pager\n\
\n\
Per avere pi? informazioni , consultare http://www.cubrid.com\n
41 %s: il valore di ambiente %s_MODE non ? corretto. \nDovrebbe essere %s o %s.\n
42 %s: troppi argomenti. Il massimo ? %d.\n
43 %s: La variabile di directory di root ($CUBRID) dovrebbe essere valorizzata.\n
44 %s: l'esecuzione di  "%s" ? fallita.\n
45 %s: -sa and -cs non si posso usare contemporaneamente.\n
46 %d comando(i) eseguiti correttamente.
47 Il comando alla linea %d,
48 Alla linea %d, colonna %d,
49 Digita `;help' per i messaggi di aiuto.
50 ERRORE: 
51 Interprete CUBRID SQL
52 Ci sono stati update. Vuoi salvare i cambiamenti ? (`Y' or `N') 
53 Digita `Y' per salvare, `N' per abbandonare. 
54 La transazione corrente ? stata salvata.
55 La transazione corrente ? stata abbandonata.
56 Buffer di comandi salvati.
57 Il file ? stato messo nel buffer dei comandi.
58 Buffer di comandi mandato alla stampante.
59 La directory corrente ? stata cambaiata in 
61 Inserire Password : 
62 <Risultato del comando %s alla linea %d>
63 Non ci sono risultati.
64 Il Checkpoint ? stato settato.
65 Il database ? ripartito.
66 Tran index      User name      Host name      Process id      Program name\n
67 %7d(%1c) %14s %14s %15d %17s\n
68 La transazione specificata ? stat cancellata.
69 Cancellazione della transazione specificata non riuscita.
70 %d righe %s.
$ These 3 should be blank
75 API automatiche di test
76 Argomento non valido per API di test
79 csql>
80 csql
81 sysadm>

145 <Aiuto: Schema delle Classi>
146 niente
147 <Aiuto: Tutti Triggers>
148 <Aiuto: Nessun Triggers Definito>
150 <Aiuto: Definizione del Trigger>
151 <Aiuto: Sintassi del Commando>
152 <Aiuto: Sommario dei comandi della Sessione>
178 Un nome file name ? richiesto.
179 Non posso eseguire il pager.
180 Memoria allocata non sufficiente
184 La linea immessa ? troppo lunga.
185 Sono state immesse troppe righe.
188 Troppi nomi file immessi.
190 Comando di sessione invalido. Digita `;help' per avere aiuto.
191 Pi? di un comando sessione possibile. Specifica meglio il comando.
192 Nessun comando. Digita `;syntax general' per la lista dei comandi disponibili.
193 Pi? di un comando sessione possibile. Specifica meglio il comando.
194 Combinazione di comandi o opzioni non valida.
196 Errore sconosciuto.
197 ';edit' non ? permesso quando output ? redirezionato.

203 <%s> 
204 <Super Classes> 
205 <Sub Classes> 
206 <Attributi> 
207 <Attributi di Classe> 
208 <Methodi> 
209 <Methodi di Classe> 
210 <Resolutions> 
211 <Method files> 
212 <Query_specifications> 
213 <Triggers> 
214 <Nome> 
215 <Evento> 
216 <Condizione Time> 
217 <Condizione> 
218 <Azione Time> 
219 <Azione> 
220 <Stato> 
221 <Priorit?> 
222 <Nome> 
223 <Descrizione> 
224 <Sintassi> 
225 <Esempio> 

$ messagi di aiuto csql
231 \
\n\
   Tutti i comandi di sessione devono essere preceduti da `;' e solo blanks/tabs\n\
   possono precedre il prefisso. Le lettere maiuscole sono l'abbreviazione minima\n\
   di cui hai bisogno per eseguire il comando.\n\
\n\
   ;REAd   [<file-name>]       - legge il file nel buffer dei commandi.\n\
   ;Write  [<file-name>]       - scrive (sovrascrive) il file con i comandi del buffer.\n\
   ;APpend [<file-name>]       - accoda ad un file i commandi del buffer.\n\
   ;PRINT                      - stampa i comandi del buffer.\n\
   ;SHELL                      - apre la shell.\n\
   ;CD                         - cambia la directory di lavoro corrente.\n\
   ;EXit                       - esce dal programma.\n\
\n\
   ;CLear                      - pulisce il buffer dei comandi.\n\
   ;EDIT                       - apre editor di sistema con i comandi del buffer.\n\
   ;List                       - mostra il contentuo del del buffer dei comandi.\n\
\n\
   ;RUn                        - esegue sql presente nel buffer dei comandi.\n\
   ;Xrun                       - esegue sql presente nel buffer dei comandi,\n\
                                 e dopo pulisce il buffer.\n\
   ;COmmit                     - conferma la transazione corrente.\n\
   ;ROllback                   - abbandona la transazione corrente.\n\
   ;AUtocommit [ON|OFF]        - abilita/disabilita modalit? auto commit.\n\
   ;REStart                    - fai ripartire il database.\n\
\n\
   ;SHELL_Cmd  [shell-cmd]     - imposta la shell, editor e il pager di default \n\
   ;EDITOR_Cmd [editor-cmd]      imposta comando nuovo o mostra quello corrente\n\
   ;PRINT_Cmd  [print-cmd]       , rispettivamente.\n\
   ;PAger_cmd  [pager-cmd]\n\
\n\
   ;DATE                       - mostra data e ora corrente.\n\
   ;DATAbase                   - mostar in nome del database in uso.\n\
   ;SChema class-name          - mostra lo schema information di una classe.\n\
   ;SYntax [sql-cmd-name]      - mostra la sintassi di un comando.\n\
   ;TRigger [`*'|trigger-name] - mostra la definizione di un trigger.\n\
   ;Get system_parameter       - prendi il valore di un parametro di sistema.\n\
   ;SEt system_parameter=value - imposta il valore di un parametro di sistema.\n\
   ;PLan [simple/detail/off]   - mostra la query execution plan.\n\
   ;Info <command>             - mostra informazioni interne.\n\
   ;TIme [ON/OFF]              - abilita/disabilita la visualizzazione del\n\
                                 execution time delle query.\n\
   ;HISTORYList                - mostra la lista delle query eseguite.\n\
   ;HISTORYRead <history_num>  - leggi una riga del history number nel buffer dei comandi.\n\
   ;HElp                       - mostra il messaggio di aiuto.\n\n

232 <Constraints> 
233 Info {schema|trigger|deferred|workspace|lock|stats}\n\
\tschema [class-name]    - informazioni dello schema\n\
\ttrigger [trigger-name] - informazioni del trigger\n\
\tdeferred               - informazioni del deferred trigger\n\
\tworkspace              - informazioni del workspace\n\
\tlock                   - informazioni dei lock (modalit? client-server)\n\
\tstats class-name       - informazioni statistiche della classe \n\
\ttrantable              - informazioni della transazione\n\
\tlogstat                - informazioni del log\n\
\tcsstat                 - informazioni della sezione critica\n\
\tplan                   - informazioni del plan cache\n\
\tqcache                 - informazioni della query cache\n
235 <Partizioni>
236 Un nome classe ? richiesto.
