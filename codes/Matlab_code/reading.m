
%%%%%%reading from a specefic equipment%%%%%%%%%
port = 'COM11';               % Adjust COM port as needed
baudRate = 115200;
s = serial(port, 'BaudRate', baudRate);
fopen(s); 
numReadings=4;
%%fscanf(s); 
while true
data1 = fgetl(s);
disp(['received: ', data1]);
if ~isempty(strfind(data1, 'ESP32_AP2'))
    fprintf(s, 'OK\n');
    break;
end
end
 while i <= numReadings
     data = fgetl(s);
disp(['donne: ', data]);
break;
end


fclose(s);                    % Always close the port when done
delete(s);                    % Clean up the object
clear s;

  %%line1 = strtrim(fscanf(s));
  %% disp(['Line 1: ', line1]);