move 0          
readc          

if (*$ == '0') then   
    set 0            
    move +1          
    jump 5           
end

move +1          
jump 1           

move 0           
if (*$ == 0) then 
    exit
end

printc           
move +1          
jump 6           