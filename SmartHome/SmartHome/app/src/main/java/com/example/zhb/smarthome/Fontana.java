package com.example.zhb.smarthome;

import android.content.Intent;
import android.os.Bundle;
import android.support.design.widget.NavigationView;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.Menu;
import android.view.MenuItem;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Toast;

import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;


public class Fontana extends AppCompatActivity implements NavigationView.OnNavigationItemSelectedListener {
    MqttAndroidClient client;
    boolean prvaPorukaPrskalicaAll = false;
    Vrednosti vr = new Vrednosti();
    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_fontana);

        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(
                this, drawer, toolbar, R.string.navigation_drawer_open, R.string.navigation_drawer_close);
        drawer.setDrawerListener(toggle);
        toggle.syncState();

        NavigationView navigationView = (NavigationView) findViewById(R.id.nav_view);
        navigationView.setNavigationItemSelectedListener(this);

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

                if (topic.equals("bozaSub/kuca/node1/prskalicaAll/stanje")) {
                    if (prvaPorukaPrskalicaAll)if (poruka.substring(0, 1).equals("0")) Toast.makeText(Fontana.this,"Prskalice su zavšile",Toast.LENGTH_LONG).show();
                    else prvaPorukaPrskalicaAll = true;
                }
            }

            @Override
            public void deliveryComplete(IMqttDeliveryToken token) {

            }
        });
    }
    private void ucitavanjeTeksta() {
        try{
            client.subscribe("bozaSub/kuca/#",0);
        }catch (MqttException e){
            e.printStackTrace();
        }
    }
    private void sendCommand(String message)
    {
        String topic = "bozaSub/kuca/node1/fontanaOsvetljenje";
        try{
            client.publish(topic,message.getBytes(),0,false);
        }catch (MqttException e){
            e.printStackTrace();
        }
    }
    public void c11(View v) {sendCommand("1");}
    public void c12(View v) {sendCommand("2");}
    public void c13(View v) {sendCommand("3");}
    public void c14(View v) {sendCommand("4");}
    public void c21(View v) {sendCommand("5");}
    public void c22(View v) {sendCommand("6");}
    public void c23(View v) {sendCommand("7");}
    public void c24(View v) {sendCommand("8");}
    public void c31(View v) {sendCommand("9");}
    public void c32(View v) {sendCommand("q");}
    public void c33(View v) {sendCommand("w");}
    public void c34(View v) {sendCommand("e");}
    public void c41(View v) {sendCommand("r");}
    public void c42(View v) {sendCommand("t");}
    public void c43(View v) {sendCommand("y");}
    public void c44(View v) {sendCommand("u");}
    public void c51(View v) {sendCommand("i");}
    public void c52(View v) {sendCommand("o");}
    public void c53(View v) {sendCommand("p");}
    public void c54(View v) {sendCommand("a");}
    public void c61(View v) {sendCommand("s");}
    public void c62(View v) {sendCommand("d");}
    public void c63(View v) {sendCommand("f");}
    public void c64(View v) {sendCommand("g");}


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
            Intent searchIntent = new Intent(Fontana.this, Settings.class);
            startActivity(searchIntent);
            overridePendingTransition(R.anim.pull_in_right, R.anim.push_out_left);
        }
        return super.onOptionsItemSelected(item);
    }

    public boolean onNavigationItemSelected(MenuItem item){
        int id = item.getItemId();

        if(id == R.id.home){
            Intent searchIntent = new Intent(Fontana.this, MainActivity.class);
            startActivity(searchIntent);
            overridePendingTransition(R.anim.pull_in_right, R.anim.push_out_left);
        }else if(id == R.id.dvoriste){
            Intent searchIntent = new Intent(Fontana.this, Dvoriste.class);
            startActivity(searchIntent);
            overridePendingTransition(R.anim.pull_in_right, R.anim.push_out_left);
        }else if(id == R.id.kapija){
            Intent searchIntent = new Intent(Fontana.this, Kapija.class);
            startActivity(searchIntent);
            overridePendingTransition(R.anim.pull_in_right, R.anim.push_out_left);
        }else if(id == R.id.fontana){
            Intent searchIntent = new Intent(Fontana.this, Fontana.class);
            startActivity(searchIntent);
            overridePendingTransition(R.anim.pull_in_right, R.anim.push_out_left);
        }else if(id == R.id.grejanje){
            Intent searchIntent = new Intent(Fontana.this, Grejanje.class);
            startActivity(searchIntent);
            overridePendingTransition(R.anim.pull_in_right, R.anim.push_out_left);
        }else if(id == R.id.klimaH){
            Intent searchIntent = new Intent(Fontana.this, KlimaH.class);
            startActivity(searchIntent);
            overridePendingTransition(R.anim.pull_in_right, R.anim.push_out_left);
        }else if(id == R.id.klimaK){
            Intent searchIntent = new Intent(Fontana.this, KlimaK.class);
            startActivity(searchIntent);
            overridePendingTransition(R.anim.pull_in_right, R.anim.push_out_left);
        }else if(id == R.id.kucica){
            Intent searchIntent = new Intent(Fontana.this, Kucica.class);
            startActivity(searchIntent);
            overridePendingTransition(R.anim.pull_in_right, R.anim.push_out_left);
        }else if(id == R.id.ruter){
            Intent searchIntent = new Intent(Fontana.this, DeviceList.class);
            startActivity(searchIntent);
            overridePendingTransition(R.anim.pull_in_right, R.anim.push_out_left);
        }
        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        drawer.closeDrawer(GravityCompat.START);
        return true;
    }
}
