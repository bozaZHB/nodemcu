package com.example.zhb.smarthome;

public class mqtt {
    Vrednosti vr;

    public void dolazecePoruke(String topic, String poruka) {
        if (topic.equals("bozaSub/kuca/node1/prskalica1/stanje")) {
            if (poruka.substring(0, 1).equals("1")) {
                vr.prskalica1 = true;
                vr.prskalica2 = false;
                vr.prskalica3 = false;
                vr.prskalica4 = false;
            }
            else if (poruka.substring(0, 1).equals("0")) vr.prskalica1 = false;
        }
        else if (topic.equals("bozaSub/kuca/node1/prskalica2/stanje")) {
            if (poruka.substring(0, 1).equals("1")) {
                vr.prskalica2 = true;
                vr.prskalica1 = false;
                vr.prskalica3 = false;
                vr.prskalica4 = false;
            }
            else if (poruka.substring(0, 1).equals("0")) vr.prskalica2 = false;
        }
        else if (topic.equals("bozaSub/kuca/node1/prskalica3/stanje")) {
            if (poruka.substring(0, 1).equals("1")) {
                vr.prskalica3 = true;
                vr.prskalica1 = false;
                vr.prskalica2 = false;
                vr.prskalica4 = false;
            }
            else if (poruka.substring(0, 1).equals("0")) vr.prskalica3 = false;
        }
        else if (topic.equals("bozaSub/kuca/node1/prskalica4/stanje")) {
            if (poruka.substring(0, 1).equals("1")) {
                vr.prskalica4 = true;
                vr.prskalica1 = false;
                vr.prskalica2 = false;
                vr.prskalica3 = false;
            }
            else if (poruka.substring(0, 1).equals("0")) vr.prskalica4 = false;
        }
        else if (topic.equals("bozaSub/kuca/node1/fontanaPumpa/stanje")) {
            if (poruka.substring(0, 1).equals("1")) vr.fontanaPrskalica = true;
            else if (poruka.substring(0, 1).equals("0")) vr.fontanaPrskalica = false;
        }
        else if (topic.equals("bozaSub/kuca/node1/radio/stanje")) {
            if (poruka.substring(0, 1).equals("1")) vr.dvoristeRadio = true;
            else if (poruka.substring(0, 1).equals("0")) vr.dvoristeRadio = false;
        }
        else if (topic.equals("bozaSub/kuca/node1/neodredjeno/stanje")) {
            if (poruka.substring(0, 1).equals("1")) vr.neodredjenoFontana = true;
            else if (poruka.substring(0, 1).equals("0")) vr.neodredjenoFontana = false;
        }
        else if (topic.equals("bozaSub/kuca/node1/prskalicaAll/stanje")) {
            if (poruka.substring(0, 1).equals("1")) vr.prskalicaAll = true;
            else if (poruka.substring(0, 1).equals("0")) {
                vr.prskalicaAll = false;
                vr.prskalica1 = false;
                vr.prskalica2 = false;
                vr.prskalica3 = false;
                vr.prskalica4 = false;
            }
        }
        else if (topic.equals("bozaSub/kuca/node1/stanje/stanje")) {
            if (poruka.substring(0, 1).equals("1")) vr.prskalica1 = true;
            else if (poruka.substring(0, 1).equals("0")) vr.prskalica1 = false;
            if (poruka.substring(1, 2).equals("1")) vr.prskalica2 = true;
            else if (poruka.substring(1, 2).equals("0")) vr.prskalica2 = false;
            if (poruka.substring(2, 3).equals("1")) vr.prskalica3 = true;
            else if (poruka.substring(2, 3).equals("0")) vr.prskalica3 = false;
            if (poruka.substring(3, 4).equals("1")) vr.prskalica4 = true;
            else if (poruka.substring(3, 4).equals("0")) vr.prskalica4 = false;
            if (poruka.substring(0, 1).equals("1") || poruka.substring(1, 2).equals("1") || poruka.substring(2, 3).equals("1") || poruka.substring(3, 4).equals("1"))
                vr.prskalicaAll = true;
            else vr.prskalicaAll = false;

            if (poruka.substring(4, 5).equals("1")) vr.fontanaPrskalica = true;
            else if (poruka.substring(4, 5).equals("0")) vr.fontanaPrskalica = false;
            if (poruka.substring(5, 6).equals("1")) vr.dvoristeRadio = true;
            else if (poruka.substring(5, 6).equals("0")) vr.dvoristeRadio = false;
            if (poruka.substring(6, 7).equals("1")) vr.neodredjenoFontana = true;
            else if (poruka.substring(6, 7).equals("0")) vr.neodredjenoFontana = false;
            if (poruka.substring(7, 8).equals("1")) vr.prskalicaTimer = true;
            else if (poruka.substring(7, 8).equals("0")) vr.prskalicaTimer = false;
        }
        else if (topic.equals("bozaSub/kuca/node1/timer")) {
            if (poruka.equals("1")) vr.prskalicaTimer = true;
            else if (poruka.equals("0")) vr.prskalicaTimer = false;
        }
        else if (topic.equals("bozaSub/kuca/node1/trajanje/stanje")) {
            vr.trajanjePrskalice = poruka;
        }
        else if (topic.equals("bozaSub/kuca/node1/vreme1/stanje") || topic.equals("bozaSub/kuca/node1/vreme1")) {
            vr.vremePrskalica1 = poruka;
        }
        else if (topic.equals("bozaSub/kuca/node1/vreme2/stanje") || topic.equals("bozaSub/kuca/node1/vreme2")) {
            vr.vremePrskalica2 = poruka;
        }
        else if (topic.equals("bozaSub/kuca/node2/dvoriste/temperatura")) {
            vr.temperaturaNapolje = poruka.substring(0, poruka.indexOf('.'))+"°C";
        }
        else if (topic.equals("bozaSub/kuca/node2/dvoriste/vlaznost")) {
            vr.vlaznostNapolje = poruka.substring(0, poruka.indexOf('.'))+"%";
        }
        else if (topic.equals("bozaSub/kuca/node2/neonka/stanje")) {
            if (poruka.equals("1")) vr.svetlo1 = true;
            else if (poruka.equals("0")) vr.svetlo1 = false;
        }
        else if (topic.equals("bozaSub/kuca/node3/vrata/stanje")) {
            if (poruka.equals("1")) vr.ulaznaVrata = true;
            else if (poruka.equals("0")) vr.ulaznaVrata = false;
        }

    }

}
