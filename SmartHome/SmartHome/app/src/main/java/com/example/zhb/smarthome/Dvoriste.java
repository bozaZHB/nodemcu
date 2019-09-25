package com.example.zhb.smarthome;

import android.app.Activity;
import android.content.Intent;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.support.design.widget.NavigationView;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

public class Dvoriste extends AppCompatActivity implements NavigationView.OnNavigationItemSelectedListener {
    TextView txttemperatura, txtvlaznost,txtvreme1, txtvreme2,txtTajmerStanje;
    ImageButton btnfontanaosvetljenje, btnfontanaprskalica, btnradio, btnulaznavrata, btnsvetlo1, btnsvetlo2,btnsvetlo3, btnsvetlo4, btnprskalice, btnPrskalica1, btnPrskalica2, btnPrskalica3, btnPrskalica4;
    Vrednosti vr;
    boolean prvaPorukaPrskalicaAll = false;
    MqttAndroidClient client;
    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_dvoriste);

        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(
                this, drawer, toolbar, R.string.navigation_drawer_open, R.string.navigation_drawer_close);
        drawer.setDrawerListener(toggle);
        toggle.syncState();

        NavigationView navigationView = (NavigationView) findViewById(R.id.nav_view);
        navigationView.setNavigationItemSelectedListener(this);

        vr = new Vrednosti();

        txttemperatura = (TextView)findViewById(R.id.txtSpoljnaTemperatura);
        txtvlaznost = (TextView)findViewById(R.id.txtVlaznostSpoljna);
        txtvreme1 = (TextView)findViewById(R.id.txtprsklaliceVreme1);
        txtvreme2 = (TextView)findViewById(R.id.txtprsklaliceVreme2);
        txtTajmerStanje = findViewById(R.id.txtTajmerStanje);
        btnfontanaosvetljenje = (ImageButton)findViewById(R.id.imageButtonFontanaSvetlo);
        btnfontanaprskalica = (ImageButton)findViewById(R.id.imageButtonFontanaPrskalica);
        btnradio = (ImageButton)findViewById(R.id.imageButtonRadio);
        btnulaznavrata = (ImageButton)findViewById(R.id.imageButtonUlaznaVrata);
        btnsvetlo1 = (ImageButton)findViewById(R.id.imageButtonSvetlo1);
        btnsvetlo2 = (ImageButton)findViewById(R.id.imageButtonSvetlo2);
        btnsvetlo3 = (ImageButton)findViewById(R.id.imageButtonSvetlo3);
        btnsvetlo4 = (ImageButton)findViewById(R.id.imageButtonSvetlo4);
        btnprskalice = (ImageButton)findViewById(R.id.imageButtonPrskalica);
        btnPrskalica1 = (ImageButton)findViewById(R.id.imageButtonPrskalica1);
        btnPrskalica2 = (ImageButton)findViewById(R.id.imageButtonPrskalica2);
        btnPrskalica3 = (ImageButton)findViewById(R.id.imageButtonPrskalica3);
        btnPrskalica4 = (ImageButton)findViewById(R.id.imageButtonPrskalica4);

        //fontana osvetljenje
        btnfontanaosvetljenje.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendCommand("bozaSub/kuca/node1/neodredjeno",vr.neodredjenoFontana==true?"0":"1" );
                //vr.neodredjenoFontana= !vr.neodredjenoFontana;
                //if (vr.neodredjenoFontana) btnfontanaosvetljenje.setBackgroundResource(R.drawable.btn_circle_on);
                //else btnfontanaosvetljenje.setBackgroundResource(R.drawable.btn_circle_off);
            }
        });

        //fontana prskalica
        btnfontanaprskalica.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendCommand("bozaSub/kuca/node1/fontanaPumpa",vr.fontanaPrskalica==true?"0":"1" );
                //vr.fontanaPrskalica= !vr.fontanaPrskalica;
                //if (vr.fontanaPrskalica) btnfontanaprskalica.setBackgroundResource(R.drawable.btn_circle_on);
                //else btnfontanaprskalica.setBackgroundResource(R.drawable.btn_circle_off);
            }
        });
        //prskalica
        btnprskalice.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                boolean stanje = vr.prskalicaAll;
                if (vr.prskalica1 || vr.prskalica2 || vr.prskalica3 || vr.prskalica4) sendCommand("bozaSub/kuca/node1/prskalicaAll","0");
                if(!stanje) sendCommand("bozaSub/kuca/node1/prskalicaAll","1" );
            }
        });
        //radio
        btnradio.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendCommand("bozaSub/kuca/node1/radio",vr.dvoristeRadio==true?"0":"1" );
                //vr.dvoristeRadio= !vr.dvoristeRadio;
                //if (vr.dvoristeRadio) btnradio.setBackgroundResource(R.drawable.btn_circle_on);
                //else btnradio.setBackgroundResource(R.drawable.btn_circle_off);
            }
        });
        //svetlo 1
        btnsvetlo1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendCommand("bozaSub/kuca/node2/neonka",vr.svetlo1==true?"0":"1" );
            }
        });
        //svetlo 2
        btnsvetlo2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                vr.svetlo2= !vr.svetlo2;
                if (vr.svetlo2) btnsvetlo2.setBackgroundResource(R.drawable.btn_circle_on);
                else btnsvetlo2.setBackgroundResource(R.drawable.btn_circle_off);
            }
        });
        //svetlo 3
        btnsvetlo3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                vr.svetlo3= !vr.svetlo3;
                if (vr.svetlo3) btnsvetlo3.setBackgroundResource(R.drawable.btn_circle_on);
                else btnsvetlo3.setBackgroundResource(R.drawable.btn_circle_off);
            }
        });
        //svetlo 4
        btnsvetlo4.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                vr.svetlo4= !vr.svetlo4;
                if (vr.svetlo4) btnsvetlo4.setBackgroundResource(R.drawable.btn_circle_on);
                else btnsvetlo4.setBackgroundResource(R.drawable.btn_circle_off);
            }
        });
        //ul vrata
        btnulaznavrata.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendCommand("bozaSub/kuca/node3/vrata",vr.ulaznaVrata==true?"0":"1" );
            }
        });
        //prskalica 1
        btnPrskalica1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                boolean stanje = vr.prskalica1;
                if (vr.prskalica1 || vr.prskalica2 || vr.prskalica3 || vr.prskalica4) sendCommand("bozaSub/kuca/node1/prskalicaAll","0");
                if(!stanje) sendCommand("bozaSub/kuca/node1/prskalica1","1" );
            }
        });
        //prskalica 2
        btnPrskalica2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                boolean stanje = vr.prskalica2;
                if (vr.prskalica1 || vr.prskalica2 || vr.prskalica3 || vr.prskalica4) sendCommand("bozaSub/kuca/node1/prskalicaAll","0");
                if(!stanje) sendCommand("bozaSub/kuca/node1/prskalica2","1" );
            }
        });
        //prskalica 3
        btnPrskalica3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                boolean stanje = vr.prskalica3;
                if (vr.prskalica1 || vr.prskalica2 || vr.prskalica3 || vr.prskalica4) sendCommand("bozaSub/kuca/node1/prskalicaAll","0");
                if(!stanje) sendCommand("bozaSub/kuca/node1/prskalica3","1" );
            }
        });
        //prskalica 4
        btnPrskalica4.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                boolean stanje = vr.prskalica4;
                if (vr.prskalica1 || vr.prskalica2 || vr.prskalica3 || vr.prskalica4) sendCommand("bozaSub/kuca/node1/prskalicaAll","0");
                if(!stanje) sendCommand("bozaSub/kuca/node1/prskalica4","1" );
            }
        });
        String clientId = MqttClient.generateClientId();
        client = new MqttAndroidClient(this.getApplicationContext(), vr.mqttBroker , clientId);//"tcp://192.168.0.17:1883"
    }

    @Override
    protected void onStart() {
        super.onStart();

        try {
            IMqttToken token = client.connect();
            token.setActionCallback(new IMqttActionListener() {
                @Override
                public void onSuccess(IMqttToken asyncActionToken) {
                    ucitavanjeTeksta();
                }

                @Override
                public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
                    //Toast.makeText(MainActivity.this,"Problem sa konekcijom sa brokerom",Toast.LENGTH_LONG).show();
                }
            });
        } catch (MqttException e) {
            e.printStackTrace();
        }

        client.setCallback(new MqttCallback() {
            @Override
            public void connectionLost(Throwable cause) {
                //Toast.makeText(MainActivity.this,"Prekid konekcije!!!",Toast.LENGTH_LONG).show();
            }

            @Override
            public void messageArrived(String topic, MqttMessage message) throws Exception {

                String poruka = new String(message.getPayload());
                mqtt mq = new mqtt();
                mq.dolazecePoruke(topic,poruka);


                if (topic.equals("bozaSub/kuca/node1/radio/stanje")) {
                    if (poruka.substring(0, 1).equals("1")) btnradio.setBackgroundResource(R.drawable.btn_circle_on);
                    else if (poruka.substring(0, 1).equals("0")) btnradio.setBackgroundResource(R.drawable.btn_circle_off);
                }
                else if (topic.equals("bozaSub/kuca/node1/neodredjeno/stanje")) {
                    if (poruka.substring(0, 1).equals("1")) btnfontanaosvetljenje.setBackgroundResource(R.drawable.btn_circle_on);
                    else if (poruka.substring(0, 1).equals("0")) btnfontanaosvetljenje.setBackgroundResource(R.drawable.btn_circle_off);
                }
                else if (topic.equals("bozaSub/kuca/node1/fontanaPumpa/stanje")) {
                    if (poruka.substring(0, 1).equals("1")) btnfontanaprskalica.setBackgroundResource(R.drawable.btn_circle_on);
                    else if (poruka.substring(0, 1).equals("0")) btnfontanaprskalica.setBackgroundResource(R.drawable.btn_circle_off);
                }
                else if (topic.equals("bozaSub/kuca/node1/vreme1/stanje") || topic.equals("bozaSub/kuca/node1/vreme1")) {
                    txtvreme1.setText(vr.vremePrskalica1);
                }
                else if (topic.equals("bozaSub/kuca/node1/vreme2/stanje") || topic.equals("bozaSub/kuca/node1/vreme2")) {
                    txtvreme2.setText(vr.vremePrskalica2);
                } else if (topic.equals("bozaSub/kuca/node1/prskalicaAll/stanje")) {
                    if (vr.prskalicaAll) btnprskalice.setBackgroundResource(R.drawable.btn_circle_on);
                    else {
                        btnprskalice.setBackgroundResource(R.drawable.btn_circle_off);
                        btnPrskalica1.setBackgroundResource(R.drawable.btn_circle_off);
                        btnPrskalica2.setBackgroundResource(R.drawable.btn_circle_off);
                        btnPrskalica3.setBackgroundResource(R.drawable.btn_circle_off);
                        btnPrskalica4.setBackgroundResource(R.drawable.btn_circle_off);
                    }
                } else if (topic.equals("bozaSub/kuca/node1/timer")) {
                    if (poruka.equals("1")) txtTajmerStanje.setText("Tajmer UKLJUČEN");
                    else if (poruka.equals("0")) txtTajmerStanje.setText("Tajmer ISKLJUČEN");
                }
                else if (topic.equals("bozaSub/kuca/node2/dvoriste/temperatura")) {
                    txttemperatura.setText(vr.temperaturaNapolje);
                }
                else if (topic.equals("bozaSub/kuca/node2/dvoriste/vlaznost")) {
                    txtvlaznost.setText(vr.vlaznostNapolje);
                }
                else if (topic.equals("bozaSub/kuca/node1/prskalica1/stanje")) {//prskalice 1
                    if (poruka.substring(0, 1).equals("1")) btnPrskalica1.setBackgroundResource(R.drawable.btn_circle_on);
                    else if (poruka.substring(0, 1).equals("0")) btnPrskalica1.setBackgroundResource(R.drawable.btn_circle_off);
                }
                else if (topic.equals("bozaSub/kuca/node1/prskalica2/stanje")) {//prskalice 2
                    if (poruka.substring(0, 1).equals("1")) btnPrskalica2.setBackgroundResource(R.drawable.btn_circle_on);
                    else if (poruka.substring(0, 1).equals("0")) btnPrskalica2.setBackgroundResource(R.drawable.btn_circle_off);
                }
                else if (topic.equals("bozaSub/kuca/node1/prskalica3/stanje")) {//prskalice 3
                    if (poruka.substring(0, 1).equals("1")) btnPrskalica3.setBackgroundResource(R.drawable.btn_circle_on);
                    else if (poruka.substring(0, 1).equals("0")) btnPrskalica3.setBackgroundResource(R.drawable.btn_circle_off);
                }
                else if (topic.equals("bozaSub/kuca/node1/prskalica4/stanje")) {//prskalice 4
                    if (poruka.substring(0, 1).equals("1")) btnPrskalica4.setBackgroundResource(R.drawable.btn_circle_on);
                    else if (poruka.substring(0, 1).equals("0")) btnPrskalica4.setBackgroundResource(R.drawable.btn_circle_off);
                }
                else if (topic.equals("bozaSub/kuca/node2/neonka/stanje")) {
                    if (poruka.substring(0, 1).equals("1")) btnsvetlo1.setBackgroundResource(R.drawable.btn_circle_on);
                    else if (poruka.substring(0, 1).equals("0")) btnsvetlo1.setBackgroundResource(R.drawable.btn_circle_off);
                }
                else if (topic.equals("bozaSub/kuca/node3/vrata/stanje")) {
                    if (poruka.substring(0, 1).equals("1")) btnulaznavrata.setBackgroundResource(R.drawable.btn_circle_on);
                    else if (poruka.substring(0, 1).equals("0")) btnulaznavrata.setBackgroundResource(R.drawable.btn_circle_off);
                }

            }

            @Override
            public void deliveryComplete(IMqttDeliveryToken token) {

            }
        });

        txttemperatura.setText(vr.temperaturaNapolje);
        txtvlaznost.setText(vr.vlaznostNapolje);
        txtvreme1.setText(vr.vremePrskalica1);
        txtvreme2.setText(vr.vremePrskalica2);
        //tekst za tajmer
        if(vr.prskalicaTimer) txtTajmerStanje.setText("Tajmer UKLJUČEN");
        else txtTajmerStanje.setText("Tajmer ISKLJUČEN");
        //prskalica
        if (vr.prskalicaAll) btnprskalice.setBackgroundResource(R.drawable.btn_circle_on);
        else btnprskalice.setBackgroundResource(R.drawable.btn_circle_off);
        //fontana prskalica
        if (vr.fontanaPrskalica) btnfontanaprskalica.setBackgroundResource(R.drawable.btn_circle_on);
        else btnfontanaprskalica.setBackgroundResource(R.drawable.btn_circle_off);
        //fontana svetlo
        if (vr.neodredjenoFontana) btnfontanaosvetljenje.setBackgroundResource(R.drawable.btn_circle_on);
        else btnfontanaosvetljenje.setBackgroundResource(R.drawable.btn_circle_off);
        //radio
        if (vr.dvoristeRadio) btnradio.setBackgroundResource(R.drawable.btn_circle_on);
        else btnradio.setBackgroundResource(R.drawable.btn_circle_off);
        //svetlo 1
        if (vr.svetlo1) btnsvetlo1.setBackgroundResource(R.drawable.btn_circle_on);
        else btnsvetlo1.setBackgroundResource(R.drawable.btn_circle_off);
        //svetlo2
        if (vr.svetlo2) btnsvetlo2.setBackgroundResource(R.drawable.btn_circle_on);
        else btnsvetlo2.setBackgroundResource(R.drawable.btn_circle_off);
        //svetlo3
        if (vr.svetlo3) btnsvetlo3.setBackgroundResource(R.drawable.btn_circle_on);
        else btnsvetlo3.setBackgroundResource(R.drawable.btn_circle_off);
        //svetlo4
        if (vr.svetlo4) btnsvetlo4.setBackgroundResource(R.drawable.btn_circle_on);
        else btnsvetlo4.setBackgroundResource(R.drawable.btn_circle_off);
        //ulazna vrata
        if (vr.ulaznaVrata) btnulaznavrata.setBackgroundResource(R.drawable.btn_circle_on);
        else btnulaznavrata.setBackgroundResource(R.drawable.btn_circle_off);
        //neonka
        if (vr.svetlo1) btnsvetlo1.setBackgroundResource(R.drawable.btn_circle_on);
        else btnsvetlo1.setBackgroundResource(R.drawable.btn_circle_off);
        //prskalice
        if (vr.prskalica1) btnPrskalica1.setBackgroundResource(R.drawable.btn_circle_on);
        else btnPrskalica1.setBackgroundResource(R.drawable.btn_circle_off);
        if (vr.prskalica2) btnPrskalica2.setBackgroundResource(R.drawable.btn_circle_on);
        else btnPrskalica2.setBackgroundResource(R.drawable.btn_circle_off);
        if (vr.prskalica3) btnPrskalica3.setBackgroundResource(R.drawable.btn_circle_on);
        else btnPrskalica3.setBackgroundResource(R.drawable.btn_circle_off);
        if (vr.prskalica4) btnPrskalica4.setBackgroundResource(R.drawable.btn_circle_on);
        else btnPrskalica4.setBackgroundResource(R.drawable.btn_circle_off);
    }
    @Override
    protected void onDestroy() {
        super.onDestroy();
        try {
            IMqttToken disconToken = client.disconnect();
            disconToken.setActionCallback(new IMqttActionListener() {
                @Override
                public void onSuccess(IMqttToken asyncActionToken) {

                }

                @Override
                public void onFailure(IMqttToken asyncActionToken, Throwable exception) {

                }
            });
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    private void ucitavanjeTeksta() {
        try{
            client.subscribe("bozaSub/kuca/#",0);
        }catch (MqttException e){
            e.printStackTrace();
        }
    }
    private void sendCommand(String topic, String message)
    {
        try{
            client.publish(topic,message.getBytes(),0,false);
        }catch (MqttException e){
            e.printStackTrace();
        }
    }

    @Override
    public void onBackPressed(){
        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        if(drawer.isDrawerOpen(GravityCompat.START)){
            drawer.closeDrawer(GravityCompat.START);
        }else{
            super.onBackPressed();
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu){
        getMenuInflater().inflate(R.menu.nav_drawer, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item){
        int id = item.getItemId();

        if(id == R.id.action_settings){
            Intent searchIntent = new Intent(Dvoriste.this, Settings.class);
            startActivity(searchIntent);
            overridePendingTransition(R.anim.pull_in_right, R.anim.push_out_left);
        }
        return super.onOptionsItemSelected(item);
    }

    public boolean onNavigationItemSelected(MenuItem item){
        int id = item.getItemId();

        if(id == R.id.home){
            Intent searchIntent = new Intent(Dvoriste.this, MainActivity.class);
            startActivity(searchIntent);
            overridePendingTransition(R.anim.pull_in_right, R.anim.push_out_left);
        }else if(id == R.id.dvoriste){
            Intent searchIntent = new Intent(Dvoriste.this, Dvoriste.class);
            startActivity(searchIntent);
            overridePendingTransition(R.anim.pull_in_right, R.anim.push_out_left);
        }else if(id == R.id.kapija){
            Intent searchIntent = new Intent(Dvoriste.this, Kapija.class);
            startActivity(searchIntent);
            overridePendingTransition(R.anim.pull_in_right, R.anim.push_out_left);
        }else if(id == R.id.fontana){
            Intent searchIntent = new Intent(Dvoriste.this, Fontana.class);
            startActivity(searchIntent);
            overridePendingTransition(R.anim.pull_in_right, R.anim.push_out_left);
        }else if(id == R.id.grejanje){
            Intent searchIntent = new Intent(Dvoriste.this, Grejanje.class);
            startActivity(searchIntent);
            overridePendingTransition(R.anim.pull_in_right, R.anim.push_out_left);
        }else if(id == R.id.klimaH){
            Intent searchIntent = new Intent(Dvoriste.this, KlimaH.class);
            startActivity(searchIntent);
            overridePendingTransition(R.anim.pull_in_right, R.anim.push_out_left);
        }else if(id == R.id.klimaK){
            Intent searchIntent = new Intent(Dvoriste.this, KlimaK.class);
            startActivity(searchIntent);
            overridePendingTransition(R.anim.pull_in_right, R.anim.push_out_left);
        }else if(id == R.id.kucica){
            Intent searchIntent = new Intent(Dvoriste.this, Kucica.class);
            startActivity(searchIntent);
            overridePendingTransition(R.anim.pull_in_right, R.anim.push_out_left);
        }else if(id == R.id.ruter){
            Intent searchIntent = new Intent(Dvoriste.this, DeviceList.class);
            startActivity(searchIntent);
            overridePendingTransition(R.anim.pull_in_right, R.anim.push_out_left);
        }
        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        drawer.closeDrawer(GravityCompat.START);
        return true;
    }
}
