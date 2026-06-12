% --- Setup serial connection ---
port = 'COM11';               % Adjust COM port as needed
baudRate = 115200;
s = serial(port, 'BaudRate', baudRate);
fopen(s);                     % Open the serial port
pause(2);                     % Wait for the device to get ready

% --- Prepare data storage ---
voltageData = [];
currentData = [];
timeData = [];

numReadings =   10;              % Number of readings to collect
i = 1;

disp(['Reading ', num2str(numReadings), ' voltage/current pairs...']);
figure(1); clf;

try
    while i <= numReadings
        % --- Read two lines from serial ---
        line1 = strtrim(fscanf(s));
        line2 = strtrim(fscanf(s));

        disp(['Line 1: ', line1]);
        disp(['Line 2: ', line2]);

        % --- Determine which line is voltage or current ---
        if ~isempty(strfind(lower(line1), 'voltage'))
            voltageLine = line1;
            currentLine = line2;
        else
            voltageLine = line2;
            currentLine = line1;
        end

        % --- Parse numeric values ---
        vMatch = regexp(voltageLine, '[Vv]oltage\s*=\s*([\d.]+)', 'tokens');
        cMatch = regexp(currentLine, '[Cc]urrent\s*=\s*([\d.]+)', 'tokens');

        if ~isempty(vMatch) && ~isempty(cMatch)
            voltage = str2double(vMatch{1}{1});
            current = str2double(cMatch{1}{1});

            % --- Store data ---
            voltageData(end+1) = voltage;
            currentData(end+1) = current;
            timeData(end+1) = i;

            % --- Plot data ---
            subplot(2, 1, 1);
            plot(timeData, voltageData, '-o');
            title('Voltage vs. Time');
            xlabel('Sample #'); ylabel('Voltage (V)');

            subplot(2, 1, 2);
            plot(timeData, currentData, '-o');
            title('Current vs. Time');
            xlabel('reading (num)'); ylabel('Current (A)');

            drawnow;

            i = i + 1;
        else
            disp('? Failed to parse voltage/current. Skipping...');
        end

        pause(0.05);  % Wait before next read
    end

    disp('? Data collection complete.');

catch ME
    disp(['? Error: ', ME.message]);
end

% --- Close serial connection ---
fclose(s);
delete(s);
clear s;
