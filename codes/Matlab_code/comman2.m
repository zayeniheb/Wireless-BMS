


%%%%read before  and after sending  sending command%%%%%%%%

port = 'COM11';                  % Adjust your COM port
baudRate = 115200;
s = serial(port, 'BaudRate', baudRate);
fopen(s);  
while true
i=0;

%disp('Waiting for ESP32_AP2...');
  while true
    if s.BytesAvailable > 0
        line = strtrim(fgetl(s));   % Remove newline characters
        disp(['Received: ', line]);

        if any(strfind(line, 'ESP32_AP2'))  % use contains() for newer MATLAB
            data = uint8(17);% send the commande that you want 
            fwrite(s,data);
            
           break;
            else if any(strfind(line, 'ESP32_AP1')) 
              data = uint8(24);
            fwrite(s,data);
             
           break;    
            % Exit loop to start receiving data
                end
        end
    end
  end
end

pause(1);   % short wait after OK

        pause(0.1);  % non-blocking wait

fclose(s);
delete(s);
clear s;
