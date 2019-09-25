package com.example.zhb.smarthome;

public class Vrednosti {
    //senzori
    public static String mqttBroker = "tcp://192.168.0.100:1883";//"tcp://192.168.0.100:1883"  "tcp://iot.eclipse.org:1883" tcp://broker.mqttdashboard.com

    public static String temperaturaNapolje = "22°C";
    public static String vlaznostNapolje = "55%";
    public static String temperaturaBoravak = "32°C";
    public static String vlaznostBoravak = "65%";
    public static String temperaturaHodnik = "52°C";
    public static String vlaznostHodnik = "75%";

    public static boolean proveraKonektivnostiNode1= false;

    public static boolean prskalicaAll = false;
    public static boolean prskalica1 = false;
    public static boolean prskalica2 = false;
    public static boolean prskalica3 = false;
    public static boolean prskalica4 = false;
    public static boolean fontanaPrskalica = false;
    public static boolean fontanaSvetlo = false;
    public static boolean garazaSpolna = false;
    public static boolean garazaUnutrasnja = false;
    public static byte kapijaIza = 0;
    public static boolean svetlo1 = false; //neonka
    public static boolean svetlo2 = false;
    public static boolean svetlo3 = false;
    public static boolean svetlo4 = false;
    public static boolean kucicaVrata = false;
    public static boolean kucicaSvetloNapolje = false;
    public static boolean kucicaSvetloUnutra = false;
    public static boolean dvoristeRadio = false;
    public static boolean kucicaRadio = false;
    public static boolean ulaznaVrata = false;
    public static boolean neodredjenoFontana = false;
    //tajmeri
    public static boolean prskalicaTimer = true;
    public static String vremePrskalica1 = "11:00";
    public static String vremePrskalica2 = "12:00";
    public static String trajanjePrskalice = "10";

    public static boolean grejanjeTimer = false;
    public static String vremeGrejanje = "10:00";
    public static String komandaGrejanje = "on";

    public static boolean klimaHodnikTimer = false;
    public static String vremeKlimaHodnik = "10:00";
    public static String komandaKlimaHodnik = "on";

    public static boolean klimaKuhinjaTimer = false;
    public static String vremeKlimaKuhinja = "10:00";
    public static String komandaKlimaKuhinja = "on";



}
