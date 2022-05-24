抱歉遲交了。

程式碼有一點亂，如果無法執行，請改用visual studio開檔案。


從上到下解釋程式碼:

BTB類別，紀錄8個BC與該列BTB的錯誤數量。
BC使用int來做儲存，並讓BC保持在0 ~ 3之間(對應SN WN WT ST)

instruction類別，記錄一個指令是addi還是beq，位置，rd1、rd2、imi與label。
imi代表imidiate，addi時才會用到這個值；
label代表要跳過去的位置，beq才會使用這個值。

changeHistory : 用來把字串Historybit改成10進制，方便直接使用後取BC的值(因為BC是用陣列儲存)。

update : 用來更新BTB與HistoryBit。

outcome : 寫得很亂的一個函式，用來幫忙出整個表格。

checkEntry : 讓使用者輸入Entry值，並確保Entry值無誤。

inputString : 讓使用者可以輸入一列又一列的指令，並把指令內容儲存到instruction裡。

run : 跑完 input String後，會開始從上到下執行指令。