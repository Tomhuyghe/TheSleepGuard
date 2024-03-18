% Charger les données
opts = detectImportOptions(heart_rate_data.csv);
opts = setvartype(opts, 'double'); % Définir le type de données pour la colonne BPM
data = readtable(heart_rate_data.csv, opts);
 
% Convertir les horodatages en format numérique
timestamps = duration(data{:,1}); % Convertir en durée
timestamps = hours(timestamps); % Convertir en heures
 
% Définir des seuils pour les phases de sommeil
WAKE_THRESHOLD = 85;
DEEP_SLEEP_THRESHOLD = 55;
bpm = data{:,2};
 
% Identifier les phases de sommeil
sleep_phase = repmat("LIGHT_SLEEP", length(bpm), 1); % Phase par défaut
sleep_phase(bpm > WAKE_THRESHOLD) = "WAKE";
sleep_phase(bpm < DEEP_SLEEP_THRESHOLD) = "DEEP_SLEEP";
 
% Analyser les transitions entre les phases
transitions = diff([0; sleep_phase ~= circshift(sleep_phase, -1); 0]);
num_transitions = sum(transitions);
 
% Estimer le nombre moyen de cycles
total_time = timestamps(end) - timestamps(1); % Temps total en heures
average_cycles = num_transitions / (total_time / 1.5); % 90 minutes (1.5 heures) par cycle
 
% Visualisation
figure;
plot(timestamps, bpm);
hold on;
 
% Colorier les phases
for i = 1:length(bpm)
    if sleep_phase(i) == "WAKE"
        scatter(timestamps(i), bpm(i), 'r', 'filled');
    elseif sleep_phase(i) == "DEEP_SLEEP"
        scatter(timestamps(i), bpm(i), 'b', 'filled');
    end
end
 
hold off;
xlabel('Temps (heures)');
ylabel('BPM');
title('Analyse des cycles de sommeil');
legend('BPM', 'WAKE', 'DEEP_SLEEP');