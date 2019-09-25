package com.example.zhb.smarthome;

import android.content.Intent;
import android.os.Bundle;
import android.support.design.widget.NavigationView;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.Spinner;
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

import static android.widget.Toast.*;

public class Settings extends AppCompatActivity implements NavigationView.OnNavigationItemSelectedListener {
    Vrednosti vr = new Vrednosti();
    LinearLayout linearLayoutVreme1, linearLayoutVreme2, linearLayoutTrajanje;
    Button btnPromeniUkljuceno, btnPromeniVreme1, btnPromeniVreme1Ok, btnVreme1Odustani ,btnPromeniVreme2, btnPromeniVreme2Ok, btnVreme2Odustani, btnPromeniTrajanje, btnPromeniTrajanjeOk, btnTrajanjeOdustani;
    EditText editVreme1, editVreme2;
    TextView txtTrenutnoStanjeUkljuceno, txtTrenutnoStanjeVreme1, txtTrenutnoStanjeVreme2, txtTrenutnoStanjeTrajanje;
    MqttAndroidClient client;
    Spinner spinnerTrajanje;
    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_settings);

        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(
                this, drawer, toolbar, R.string.navigation_drawer_open, R.string.navigation_drawer_close);
        drawer.setDrawerListener(toggle);
        toggle.syncState();

        NavigationView navigationView = (NavigationView) findViewById(R.id.nav_view);
        navigationView.setNavigationItemSelectedListener(this);
        spinnerTrajanje = findViewById(R.id.spinerTrajanje);
        linearLayoutVreme1 = findViewById(R.id.linearLayoutVreme1);
        linearLayoutVreme2 = findViewById(R.id.linearLayoutVreme2);
        linearLayoutTrajanje = findViewById(R.id.linearLayoutTrajanje);
        btnPromeniUkljuceno = findViewById(R.id.btnPromeniUkljuceno);
        btnPromeniVreme1 = findViewById(R.id.btnPromeniVreme1);
        btnVreme1Odustani = findViewById(R.id.btnVreme1Odustani);
        btnPromeniVreme2 = findViewById(R.id.btnPromeniVreme2);
        btnVreme2Odustani = findViewById(R.id.btnVreme2Odustani);
        btnPromeniTrajanje = findViewById(R.id.btnPromeniTrajanje);
        btnTrajanjeOdustani = findViewById(R.id.btnTrajanjeOdustani);
        editVreme1 = findViewById(R.id.editVreme1);
        editVreme2 = findViewById(R.id.editVreme2);
        txtTrenutnoStanjeUkljuceno = findViewById(R.id.txtTrenutnoStanjeUkljuceno);
        txtTrenutnoStanjeVreme1 = findViewById(R.id.txtTrenutnoStanjeVreme1);
        txtTrenutnoStanjeVreme2 = findViewById(R.id.txtTrenutnoStanjeVreme2);
        txtTrenutnoStanjeTrajanje = findViewById(R.id.txtTrenutnoStanjeTrajanje);
        linearLayoutVreme1.setVisibility(View.GONE);
        linearLayoutVreme2.setVisibility(View.GONE);
        linearLayoutTrajanje.setVisibility(View.GONE);

        btnPromeniUkljuceno.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                vr.prskalicaTimer = !vr.prskalicaTimer;
                String temp;
                if (vr.prskalicaTimer) {
                    temp = "uključeno";
                    btnPromeniVreme1.setEnabled(true);
                    btnPromeniVreme2.setEnabled(true);
                    btnPromeniTrajanje.setEnabled(true);
                }
                else {
                    temp = "isključeno";
                    btnPromeniVreme1.setEnabled(false);
                    btnPromeniVreme2.setEnabled(false);
                    btnPromeniTrajanje.setEnabled(false);
                    linearLayoutVreme1.setVisibility(View.GONE);
                    linearLayoutVreme2.setVisibility(View.GONE);
                    linearLayoutTrajanje.setVisibility(View.GONE);
                }
                txtTrenutnoStanjeUkljuceno.setText("Trenutno stanje "+temp);
            }
        });

        btnPromeniVreme1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                linearLayoutVreme1.setVisibility(View.VISIBLE);
            }
        });
        btnPromeniVreme2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                linearLayoutVreme2.setVisibility(View.VISIBLE);
            }
        });
        btnPromeniTrajanje.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                linearLayoutTrajanje.setVisibility(View.VISIBLE);
            }
        });
        btnVreme1Odustani.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                linearLayoutVreme1.setVisibility(View.GONE);
            }
        });
        btnVreme2Odustani.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                linearLayoutVreme2.setVisibility(View.GONE);
            }
        });
        btnTrajanjeOdustani.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                linearLayoutTrajanje.setVisibility(View.GONE);
            }
        });

        String clientId = MqttClient.generateClientId();
        client = new MqttAndroidClient(this.getApplicationContext(), vr.mqttBroker , clientId);//"tcp://192.168.0.17:1883"


        if(vr.trajanjePrskalice.equals("10")) spinnerTrajanje.setSelection(0);
        else if (vr.trajanjePrskalice.equals("15")) spinnerTrajanje.setSelection(1);
        else if (vr.trajanjePrskalice.equals("20")) spinnerTrajanje.setSelection(2);
    }

    public void potvrdi_tajmer_dugme(View view){
        if(txtTrenutnoStanjeUkljuceno.getText().equals("Trenutno stanje uključeno")){
            sendCommand("bozaSub/kuca/node1/timer","1");
            sendCommand("bozaSub/kuca/node1/vreme1",editVreme1.getText().toString());
            sendCommand("bozaSub/kuca/node1/vreme2",editVreme2.getText().toString());
            //sendCommand("bozaSub/kuca/node1/trajanje",editTrajanje.getText().toString());
            if (spinnerTrajanje.getSelectedItem().toString().equals("10min"))sendCommand("bozaSub/kuca/node1/trajanje","1");
            else if (spinnerTrajanje.getSelectedItem().toString().equals("15min"))sendCommand("bozaSub/kuca/node1/trajanje","2");
            else if (spinnerTrajanje.getSelectedItem().toString().equals("20min"))sendCommand("bozaSub/kuca/node1/trajanje","3");
        }
        else{
            sendCommand("bozaSub/kuca/node1/timer","0");
        }
        linearLayoutVreme1.setVisibility(View.GONE);
        linearLayoutVreme2.setVisibility(View.GONE);
        linearLayoutTrajanje.setVisibility(View.GONE);
    }
    private void sendCommand(String topic, String message)
    {
        try{
            client.publish(topic,message.getBytes(),0,false);
        }catch (MqttException e){
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

    @Override
    protected void onStart() {
        super.onStart();

        editVreme1.setText(vr.vremePrskalica1);
        editVreme2.setText(vr.vremePrskalica2);

        String temp;
        if (vr.prskalicaTimer) {
            temp = "uključeno";
            btnPromeniVreme1.setEnabled(true);
            btnPromeniVreme2.setEnabled(true);
            btnPromeniTrajanje.setEnabled(true);
        }
        else {
            temp = "isključeno";
            btnPromeniVreme1.setEnabled(false);
            btnPromeniVreme2.setEnabled(false);
            btnPromeniTrajanje.setEnabled(false);
            linearLayoutVreme1.setVisibility(View.GONE);
            linearLayoutVreme2.setVisibility(View.GONE);
            linearLayoutTrajanje.setVisibility(View.GONE);
        }
        txtTrenutnoStanjeUkljuceno.setText("Trenutno stanje "+temp);
        txtTrenutnoStanjeVreme1.setText("Prvo uključivanje prskalice u " +vr.vremePrskalica1+"h");
        txtTrenutnoStanjeVreme2.setText("Drugo uključivanje prskalice u " +vr.vremePrskalica2+"h");
        txtTrenutnoStanjeTrajanje.setText("Trenutno trajanje prskalice "+vr.trajanjePrskalice +" min");

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


                if (topic.equals("bozaSub/kuca/node1/timer")) {
                    if (poruka.equals("1")) {
                        btnPromeniVreme1.setEnabled(true);
                        btnPromeniVreme2.setEnabled(true);
                        btnPromeniTrajanje.setEnabled(true);
                        txtTrenutnoStanjeUkljuceno.setText("Trenutno stanje "+"uključeno");
                    }
                    else if (poruka.equals("0")) {
                        btnPromeniVreme1.setEnabled(false);
                        btnPromeniVreme2.setEnabled(false);
                        btnPromeniTrajanje.setEnabled(false);
                        linearLayoutVreme1.setVisibility(View.GONE);
                        linearLayoutVreme2.setVisibility(View.GONE);
                        linearLayoutTrajanje.setVisibility(View.GONE);
                        txtTrenutnoStanjeUkljuceno.setText("Trenutno stanje "+"isključeno");
                    }
                }
                else if (topic.equals("bozaSub/kuca/node1/trajanje/stanje") || topic.equals("bozaSub/kuca/node1/trajanje")) {
                    txtTrenutnoStanjeTrajanje.setText("Trenutno trajanje prskalice "+vr.trajanjePrskalice +" min");
                }
                else if (topic.equals("bozaSub/kuca/node1/vreme1/stanje") || topic.equals("bozaSub/kuca/node1/vreme1")) {
                    txtTrenutnoStanjeVreme1.setText("Prvo uključivanje prskalice u " +vr.vremePrskalica1+"h");
                    editVreme1.setText(vr.vremePrskalica1);
                }
                else if (topic.equals("bozaSub/kuca/node1/vreme2/stanje") || topic.equals("bozaSub/kuca/node1/vreme2")) {
                    txtTrenutnoStanjeVreme2.setText("Drugo uključivanje prskalice u " +vr.vremePrskalica2+"h");
                    editVreme2.setText(vr.vremePrskalica2);
                }

                if(vr.trajanjePrskalice.equals("10")) spinnerTrajanje.setSelection(0);
                else if (vr.trajanjePrskalice.equals("15")) spinnerTrajanje.setSelection(1);
                else if (vr.trajanjePrskalice.equals("20")) spinnerTrajanje.setSelection(2);
            }

            @Override
            public void deliveryComplete(IMqttDeliveryToken token) {

            }
        });
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


    public boolean onNavigationItemSelected(MenuItem item){
        int id = item.getItemId();

        if(id == R.id.home){
            Intent searchIntent = new Intent(Settings.this, MainActivity.class);
            startActivity(searchIntent);
            overridePendingTransition(R.anim.pull_in_right, R.anim.push_out_left);
        }else if(id == R.id.dvoriste){
            Intent searchIntent = new Intent(Settings.this, Dvoriste.class);
            startActivity(searchIntent);
            overridePendingTransition(R.anim.pull_in_right, R.anim.push_out_left);
        }else if(id == R.id.kapija){
            Intent searchIntent = new Intent(Settings.this, Kapija.class);
            startActivity(searchIntent);
            overridePendingTransition(R.anim.pull_in_right, R.anim.push_out_left);
        }else if(id == R.id.fontana){
            Intent searchIntent = new Intent(Settings.this, Fontana.class);
            startActivity(searchIntent);
            overridePendingTransition(R.anim.pull_in_right, R.anim.push_out_left);
        }else if(id == R.id.grejanje){
            Intent searchIntent = new Intent(Settings.this, Grejanje.class);
            startActivity(searchIntent);
            overridePendingTransition(R.anim.pull_in_right, R.anim.push_out_left);
        }else if(id == R.id.klimaH){
            Intent searchIntent = new Intent(Settings.this, KlimaH.class);
            startActivity(searchIntent);
            overridePendingTransition(R.anim.pull_in_right, R.anim.push_out_left);
        }else if(id == R.id.klimaK){
            Intent searchIntent = new Intent(Settings.this, KlimaK.class);
            startActivity(searchIntent);
            overridePendingTransition(R.anim.pull_in_right, R.anim.push_out_left);
        }else if(id == R.id.kucica){
            Intent searchIntent = new Intent(Settings.this, Settings.class);
            startActivity(searchIntent);
            overridePendingTransition(R.anim.pull_in_right, R.anim.push_out_left);
        }else if(id == R.id.ruter){
            Intent searchIntent = new Intent(Settings.this, DeviceList.class);
            startActivity(searchIntent);
            overridePendingTransition(R.anim.pull_in_right, R.anim.push_out_left);
        }
        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        drawer.closeDrawer(GravityCompat.START);
        return true;
    }
}
