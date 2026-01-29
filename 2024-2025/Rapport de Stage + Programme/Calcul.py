import csv

input_files = [
    "releve_experimental_test.csv",
    "releve_experimental_training.csv"
]
output_files = [
    "releve_experimental_test_out.csv",
    "releve_experimental_train_out.csv"
]

for input_file, output_file in zip(input_files, output_files):
    with open(input_file, newline='', encoding='utf-8') as infile, \
         open(output_file, 'w', newline='', encoding='utf-8') as outfile:
        reader = csv.DictReader(infile)
        fieldnames = (list(reader.fieldnames) if reader.fieldnames is not None else []) + [
            'calc_preference_recall',
            'calc_indifference_recall',
            'calc_preference_precision',
            'calc_indifference_precision',
            'calc_precision_total'
        ]
        writer = csv.DictWriter(outfile, fieldnames=fieldnames)
        writer.writeheader()
        for row in reader:
            # Convert needed fields to float or int
            nb_pref_vrai = float(row['nb_pref_vrai'])
            nb_pref_fausse = float(row['nb_pref_fausse'])
            nb_pref_inverse = float(row['nb_pref_inverse'])
            nb_indiff_vrai = float(row['nb_indiff_vrai'])
            nb_indiff_fausse = float(row['nb_indiff_fausse'])
            nb_indifferences = float(row['nb_indifferences'])
            nb_preferences = float(row['nb_preferences'])

            # Calculs
            denom_pref_recall = nb_pref_vrai + nb_pref_fausse + nb_pref_inverse
            denom_indiff_recall = nb_indiff_vrai + nb_indiff_fausse
            denom_pref_precision = nb_pref_vrai + nb_pref_fausse
            denom_indiff_precision = nb_indiff_vrai + nb_pref_inverse
            total = nb_indifferences + nb_preferences
            correct = nb_pref_vrai + nb_indiff_vrai

            row['calc_preference_recall'] = nb_pref_vrai / denom_pref_recall if denom_pref_recall > 0 else 0
            row['calc_indifference_recall'] = nb_indiff_vrai / denom_indiff_recall if denom_indiff_recall > 0 else 0
            row['calc_preference_precision'] = nb_pref_vrai / (denom_pref_precision + nb_indiff_fausse) if denom_pref_precision > 0 else 0
            row['calc_indifference_precision'] = nb_indiff_vrai / denom_indiff_precision if denom_indiff_precision > 0 else 0
            row['calc_precision_total'] = correct / total if total > 0 else 0

            writer.writerow(row)