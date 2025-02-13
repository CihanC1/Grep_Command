#!/bin/bash

# Ana klasör adı
MAIN_DIR="Projekt"

# Eğer klasör zaten varsa, sil ve yeniden oluştur
rm -rf "$MAIN_DIR"
mkdir "$MAIN_DIR"

# Almanca metin kaynağı (örnek makaleler)
GERMAN_TEXTS=(
    "Die künstliche Intelligenz hat in den letzten Jahren erhebliche Fortschritte gemacht. Sie wird in vielen Bereichen eingesetzt."
    "Die Automobilindustrie entwickelt selbstfahrende Autos, die den Verkehr revolutionieren können."
    "Deutschland ist bekannt für seine Ingenieurskunst und hochwertige Maschinenbauprodukte."
    "Die Energiewende in Deutschland fördert den Ausbau erneuerbarer Energien wie Wind- und Solarenergie."
    "Goethe und Schiller gehören zu den berühmtesten deutschen Dichtern und Denkern."
    "Die deutsche Sprache hat viele Dialekte, die sich von Region zu Region unterscheiden."
    "Die Geschichte Deutschlands ist durch viele bedeutende Ereignisse geprägt."
    "Die deutsche Wirtschaft ist eine der stärksten in Europa und weltweit führend in vielen Industriezweigen."
    "Deutschland ist für seine Vielfalt an Landschaften bekannt, von den Alpen bis zur Nordsee."
    "Die Wissenschaft in Deutschland hat viele Nobelpreisträger hervorgebracht."
)

# 20 klasör oluştur
for i in {1..20}
do
    FOLDER="$MAIN_DIR/Ordner_$i"
    mkdir "$FOLDER"

    # 20 dosya oluştur ve içine metin ekle
    for j in {1..20}
    do
        FILE="$FOLDER/Datei_$j.txt"

        # 20 satır Almanca metin ekle
        for k in {1..20}
        do
            RANDOM_INDEX=$(( RANDOM % ${#GERMAN_TEXTS[@]} ))  # Rastgele bir metin seç
            echo "${GERMAN_TEXTS[$RANDOM_INDEX]}" >> "$FILE"
        done
    done
done

echo "📂 $MAIN_DIR klasörü oluşturuldu, içindeki dosyalar gerçek Almanca metinler ile dolduruldu! ✅"

