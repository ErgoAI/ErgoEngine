/*
 * Copyright 2015-2016, Coherent Knowledge Systems.
 *
 * All rights reserved.
 */ 
package com.coherentknowledge.ergo.sparql.gui;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.io.PrintWriter;
import java.net.NoRouteToHostException;
import java.net.UnknownHostException;
import java.util.Calendar;
import java.util.GregorianCalendar;
import java.util.Properties;

import org.apache.jena.atlas.web.HttpException;
import org.apache.log4j.Level;
import org.apache.log4j.Logger;

import com.hp.hpl.jena.query.QueryException;
import com.hp.hpl.jena.sparql.engine.http.QueryExceptionHTTP;

import javafx.application.Application;
import javafx.application.Platform;
import javafx.beans.binding.Bindings;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.concurrent.Service;
import javafx.concurrent.Task;
import javafx.event.ActionEvent;
import javafx.event.Event;
import javafx.event.EventHandler;
import javafx.scene.Cursor;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.GridPane;
import javafx.scene.text.Font;
import javafx.stage.FileChooser;
import javafx.stage.Modality;
import javafx.stage.Stage;
import javafx.geometry.Pos;
import javafx.scene.control.ComboBox;
import javafx.scene.control.Label;
import javafx.scene.control.PasswordField;
import javafx.scene.control.RadioButton;
import javafx.scene.control.TextArea;
import javafx.scene.control.TextField;
import javafx.scene.control.ToggleGroup;
import javafx.scene.layout.FlowPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.StackPane;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.control.Menu;
import javafx.scene.control.MenuBar;
import javafx.scene.control.MenuItem;
import javafx.scene.input.KeyCombination;

import com.coherentknowledge.ergo.sparql.*;

/**
 *
 * @author Paul Fodor
 */
public class ErgoSPARQL_GUI extends Application {
        
    String versionString = "0.7.5 (July 5, 2015)";
    Label statusLabel = new Label("");

    // SPARQL endpoint
    TextField sparqlEndpointTextField = new TextField();
    TextField userTextField = new TextField();
    PasswordField passwordTextField = new PasswordField();
        
    // IRIs
    TextArea irisTextArea = new TextArea();

    // query
    TextArea queryTextArea = new TextArea();
        
    // SPARQL predicate
    TextField predicateTextField = new TextField();

    // output format
    RadioButton fastloadOutputRadioButton;
    RadioButton ergoOutputRadioButton;
    ToggleGroup formatRadioGroup = new ToggleGroup();

    // center output
    Label ergoTopLabel = new Label("");;
    File outputFile;

    TextArea ergoTextArea = new TextArea();
    String ergoText = "";
        
    Stage primaryStage;
        
    static boolean debugFlag = false;
        
    @Override
    public void start(Stage primaryStage) {
        this.primaryStage = primaryStage;
                
        // Create GUI
        MenuBar menuBar = new MenuBar();
        Menu fileOperations = new Menu("File");
        Menu helpOperations = new Menu("Help");
        menuBar.getMenus().addAll(fileOperations, helpOperations);

        MenuItem menuItemReset = new MenuItem("Reset Query");
        MenuItem menuItemSave = new MenuItem("Save Query");
        MenuItem menuItemLoad = new MenuItem("Load Query");
        fileOperations.getItems().addAll(menuItemSave, menuItemLoad, menuItemReset);

        menuItemReset.setAccelerator(KeyCombination.keyCombination("Ctrl+N"));
        menuItemReset.setOnAction( e -> resetQuery() );

        menuItemSave.setAccelerator(KeyCombination.keyCombination("Ctrl+S"));
        menuItemSave.setOnAction( e -> saveLastQueryInPropertiesFile() );

        menuItemLoad.setAccelerator(KeyCombination.keyCombination("Ctrl+L"));
        menuItemLoad.setOnAction( e -> loadQuery() );
                
        MenuItem menuItemAbout = new MenuItem("About");
        helpOperations.getItems().addAll(menuItemAbout);

        menuItemAbout.setAccelerator(KeyCombination.keyCombination("F1"));              
        menuItemAbout.setOnAction( e -> helpDialog(primaryStage) );
                
        StackPane top = new StackPane();

        VBox left = new VBox();
        Label subTitleLabel;
        
        subTitleLabel = new Label("   Query SPARQL endpoints");

        // testing the current directory
        //subTitleLabel = new Label( System.getProperty("user.dir") );
        
        subTitleLabel.setFont(new Font(20));
        left.getChildren().add(subTitleLabel);
                
        // Status label
        FlowPane statusPane = new FlowPane();
        statusLabel.setFont(new Font(20));
        statusLabel.setStyle("-fx-text-fill: red; -fx-font-weight:bolder");
        statusLabel.setText("Status: Ready");
        statusPane.getChildren().addAll(statusLabel);
        left.getChildren().add(statusPane);
                                
        if(!lastPropertiesExist){
            // DBPEDIA query --------------------------------------------
            resetQuery();
            // default output file is in the current user directory
            outputFile = new File(System.getProperty("user.dir") + File.separator + "sparqlResult.P");
        }else{
            try{
                sparqlEndpointTextField.setText(lastURL);
                userTextField.setText(lastUser);
                passwordTextField.setText(lastPassword);
                ErgoSPARQL.setIris(lastPreffixes);
                irisTextArea.setText(ErgoSPARQL.getIrisText());
                queryTextArea.setText(lastQuery);
                outputFile = new File(lastOutputFile);
            }catch(IllegalArgumentException e){
                e.printStackTrace();
                System.out.println("Error in the lastQuery.properties file: " + lastPreffixes);
                resetQuery();
            }catch(Exception e){
                e.printStackTrace();
                System.out.println("Error in the lastQuery.properties file: " + lastOutputFile);
                resetQuery();
                outputFile = new File(System.getProperty("user.dir") + File.separator + "sparqlResult.P");
            }
        }
                
        // select input endpoint
        VBox inputPane = new VBox();
        inputPane.setStyle("-fx-color: lightblue; -fx-border-color: blue; -fx-border-width: 2");

        Label inputLabel = new Label("Enter SPARQL endpoint location:");
        //inputLabel.setFont(new Font(20));
        inputPane.getChildren().add(inputLabel);
                                
        inputPane.getChildren().add(sparqlEndpointTextField);

        Label userLabel = new Label("SPARQL endpoint user name: ");
        inputPane.getChildren().add(userLabel);
        inputPane.getChildren().add(userTextField);

        Label passwordLabel = new Label("SPARQL endpoint password: ");
        inputPane.getChildren().add(passwordLabel);
        inputPane.getChildren().add(passwordTextField);
        left.getChildren().add(inputPane);

        // SPARQL query
        VBox queryPane = new VBox();
        queryPane.setStyle("-fx-color: lightblue; -fx-border-color: red; -fx-border-width: 2");

        // manage iris
        Label manageIRIsLabel = new Label("SPARQL prefixes:");
        queryPane.getChildren().add(manageIRIsLabel);
        irisTextArea.setPrefRowCount(6);
        irisTextArea.setPrefColumnCount(10);
        queryPane.getChildren().add(irisTextArea);
                
        Label outputLabel = new Label("SPARQL query:");
        queryPane.getChildren().add(outputLabel);

        queryPane.getChildren().add(queryTextArea);
        left.getChildren().add(queryPane);

        // Select the output
        VBox outputFormatPane = new VBox();
        outputFormatPane.setStyle("-fx-color: lightblue; -fx-border-color: blue; -fx-border-width: 2");

        Label outputFormatLabel = new Label("Output format (fastload .P or .ergo):");
        outputFormatLabel.setFont(new Font(20));
                
        fastloadOutputRadioButton = new RadioButton("fastload format");
        ergoOutputRadioButton = new RadioButton("Ergo format");
        fastloadOutputRadioButton.setToggleGroup(formatRadioGroup);
        ergoOutputRadioButton.setToggleGroup(formatRadioGroup);
                
        // set the default fastload to true
        fastloadOutputRadioButton.setSelected(true);
        ErgoSPARQL.outputFormat = 1;
                
        fastloadOutputRadioButton.setOnAction(new EventHandler<ActionEvent>(){ public void handle(ActionEvent e){ ErgoSPARQL.outputFormat = 1; }});
        ergoOutputRadioButton.setOnAction(new EventHandler<ActionEvent>(){ public void handle(ActionEvent e){ ErgoSPARQL.outputFormat = 2; }});
                
        outputFormatPane.getChildren().addAll(outputFormatLabel, fastloadOutputRadioButton, ergoOutputRadioButton);
                
        left.getChildren().add(outputFormatPane);
                
        // predicate name
        VBox predicateNamePane = new VBox();
        predicateNamePane.setStyle("-fx-color: lightblue; -fx-border-color: blue; -fx-border-width: 2");
                
        Label ergoPredicateLabel = new Label("Output predicate name:");
        ergoPredicateLabel.setFont(new Font(20));
        predicateTextField = new TextField();
        predicateTextField.setPromptText("Predicate name ('node' is the default)");
                
        predicateNamePane.getChildren().addAll(ergoPredicateLabel, predicateTextField);
        left.getChildren().add(predicateNamePane);
                
        // translate
        VBox queryButtonPane = new VBox();
        queryButtonPane.setAlignment(Pos.CENTER);
        Button queryButton = new Button("Query SPARQL endpoint");
        queryButton.setStyle("-fx-color: pink; -fx-border-color: red; -fx-border-width: 4");
        queryButton.setFont(new Font(20));
                
        queryButton.disableProperty().bind(service.runningProperty());
                
        queryButton.setOnAction(
                                new EventHandler<ActionEvent>(){ 
                                    public void handle(ActionEvent e){ 
                                        try{
                                            // save iris
                                            String irisString = irisTextArea.getText();
                                            ErgoSPARQL.setIris(irisString);
                                            service.restart();
                                        }catch(IllegalArgumentException e2){
                                            e2.printStackTrace();
                                            statusLabel.setText("Status: Error in IRI prefixes");
                                        }
                                    }});
        queryButtonPane.getChildren().addAll(queryButton);
        left.getChildren().addAll(queryButtonPane);
                
        // CENTER Pane
        VBox center = new VBox();
        ergoTopLabel.setText("Output file: " + outputFile.getAbsolutePath());

        Button changeOutputFile = new Button("Change output file location");
        changeOutputFile.setOnAction( e-> changeOutputFile() );
                
        center.getChildren().addAll(ergoTopLabel, changeOutputFile);

        ergoTextArea.setPrefRowCount(150);
        // ergoTA.setPrefColumnCount(20);
        center.getChildren().add(ergoTextArea);

        // BOTTOM Pane
        FlowPane bottom = new FlowPane();
        // bottom.setPadding(new Insets(11.5, 12.5, 13.5, 14.5));

        try{
	        Image image = new Image(getClass().getResourceAsStream("Ergo/ergo_lib/ergo2owl/ck.png"));
	        ImageView imageView = new ImageView(image);
	        imageView.setFitHeight(50);
	        imageView.setFitWidth(90);
	        bottom.getChildren().addAll(imageView);
        }catch(Exception e){
        	// image was not found 
        	System.out.println("ErgoSPARQL_GUI warning: image ck.png was not found");
        }
        
        Label copyrightVersionLabel = new Label(
                                                "     @Copyright 2015, Coherent Knowledge Systems, Ergo/SPARQL Query Tool, version " 
                                                + versionString
                                                + "                                                    ");
        copyrightVersionLabel.setFont(new Font(14));
        Button helpButton = new Button("Help");
        helpButton.setStyle("-fx-color: lightgreen; -fx-border-color: green; -fx-border-width: 2");
        helpButton.setTextFill(Color.RED);
        helpButton.setFont(new Font(20));
        helpButton.setOnAction(new EventHandler<ActionEvent>(){ public void handle(ActionEvent e){ helpDialog(primaryStage); }});

        bottom.getChildren().addAll(copyrightVersionLabel); // , helpButton

        // MAIN Pane
        VBox rootNode = new VBox(10);
        BorderPane contentNode = new BorderPane();
        contentNode.setTop(top);
        contentNode.setBottom(bottom);
        contentNode.setLeft(left);
        contentNode.setCenter(center);

        rootNode.getChildren().addAll(menuBar, contentNode);
                
        Scene scene = new Scene(rootNode, 1200, 950);

        primaryStage.setTitle("Ergo SPARQL");
        primaryStage.setScene(scene);
                
        primaryStage.getScene().getRoot()
            .cursorProperty()
            .bind(Bindings.when(service.runningProperty())
                  .then(Cursor.WAIT).otherwise(Cursor.DEFAULT));
                
        primaryStage.show();
    }
        
    public void changeOutputFile(){
        FileChooser outputFileChooser = new FileChooser();
        outputFileChooser.setTitle("Change Output File");
        String userDir = System.getProperty("user.dir") + File.separator;
        outputFileChooser.setInitialDirectory(new File(userDir));
        if(ErgoSPARQL.outputFormat == 1)
            outputFileChooser.setInitialFileName("sparqlResult.P");
        else
            outputFileChooser.setInitialFileName("sparqlResult.ergo");
        File newOutputFile = outputFileChooser.showSaveDialog(null);
        if(newOutputFile != null){
            outputFile = newOutputFile;
            ergoTopLabel.setText("Output file: " + outputFile.getAbsolutePath());
        }
    }
        
    public void saveLastQueryInPropertiesFile(){
        FileChooser outputFileChooser = new FileChooser();
        outputFileChooser.setTitle("Save Query");
        outputFileChooser.setInitialDirectory(sparqlDir);
        outputFileChooser.setInitialFileName("query.properties");
        File propertiesFile = outputFileChooser.showSaveDialog(null); 
        saveQuery(propertiesFile.getAbsolutePath());
    }
        
    public void saveQuery(String propertiesFile) {
        // get the last values from the UI
        lastURL = sparqlEndpointTextField.getText();
        lastUser = userTextField.getText();
        lastPassword = passwordTextField.getText();
        lastPreffixes = irisTextArea.getText();
        System.out.println(lastPreffixes);
        lastQuery = queryTextArea.getText();
        lastOutputFile = outputFile.getAbsolutePath();
        // save the properties
        Properties prop = new Properties();
        OutputStream output = null;
        try {
            output = new FileOutputStream(propertiesFile);
            // set the properties value
            prop.setProperty("lastURL", lastURL);
            prop.setProperty("lastUser", lastUser);
            prop.setProperty("lastPassword", lastPassword);
            prop.setProperty("lastPreffixes", lastPreffixes);
            prop.setProperty("lastQuery", lastQuery);
            prop.setProperty("lastOutputFile", lastOutputFile);
            // save properties to project root folder
            prop.store(output, null);
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (output != null) {
                try {
                    output.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }

    }
        
    public void loadQuery(){
        FileChooser outputFileChooser = new FileChooser();
        outputFileChooser.setTitle("Load Query");
        outputFileChooser.setInitialDirectory(sparqlDir);
        outputFileChooser.setInitialFileName("lastQuery.properties");
        File propertiesFile = outputFileChooser.showOpenDialog(null); 
        loadLastQueryFromPropertyFile(propertiesFile.getAbsolutePath());
        // update the GUI 
        try{
            sparqlEndpointTextField.setText(lastURL);
            userTextField.setText(lastUser);
            passwordTextField.setText(lastPassword);
            ErgoSPARQL.setIris(lastPreffixes);
            irisTextArea.setText(ErgoSPARQL.getIrisText());
            queryTextArea.setText(lastQuery);
            ergoTopLabel.setText("Output file: " + outputFile.getAbsolutePath());
        }catch(IllegalArgumentException e){
            e.printStackTrace();
            System.out.println("Error in the lastQuery.properties file: " + lastPreffixes);
            resetQuery();
        }
    }
        
    public static void loadLastQueryFromPropertyFile(String propertiesFile){
        Properties prop = new Properties();
        InputStream input = null;
        try {
            input = new FileInputStream(propertiesFile);
            // load the properties file
            prop.load(input);
            // get the property value and print it out
            lastURL = prop.getProperty("lastURL");
            lastUser = prop.getProperty("lastUser");
            lastPassword = prop.getProperty("lastPassword");
            lastPreffixes = prop.getProperty("lastPreffixes");
            lastQuery = prop.getProperty("lastQuery");
            lastOutputFile = prop.getProperty("lastOutputFile");
            lastPropertiesExist = true;
            // logging
            /*
              log.println("Last URL: " + lastURL);
              log.println("Last user: " + lastUser);
              log.println("lastPreffixes: " + lastPreffixes);
              log.println("Last Query: " + lastQuery);
            */
        } catch (IOException e) {
            if(debugFlag)
                e.printStackTrace();
            log.println("Could not find the last lastQuery.properties file.");
        } finally {
            if (input != null) {
                try {
                    input.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }
        
    public void resetQuery(){
        // DBPEDIA query --------------------------------------------
        sparqlEndpointTextField.setText("http://dbpedia.org/sparql");
        userTextField.setText("");
        passwordTextField.setText("");
        ErgoSPARQL.initializeIrisDBPedia();
        irisTextArea.setText(ErgoSPARQL.getIrisText());
        queryTextArea.setText(
                              "SELECT ?s ?p ?v \n"
                              + "WHERE{ \n" 
                              + "     ?s ?p ?v \n"
                              + "} \n"
                              + "LIMIT 100 \n");
        // -------------------------------------------------------------
    }
    public void helpDialog(Stage primaryStage) {
        Stage dialogStage = new Stage();
        dialogStage.initModality(Modality.WINDOW_MODAL);
        dialogStage.initOwner(primaryStage);
        BorderPane helpPane = new BorderPane();
        HBox optionPane = new HBox();
        Button nextButton = new Button("Ok");
        optionPane.setSpacing(10.0);
        optionPane.getChildren().addAll(nextButton);
        
        try{
	        Image image = new Image(getClass().getResourceAsStream("Ergo/ergo_lib/ergo2owl/ck.png"));
	        ImageView logoImageView = new ImageView(image);
	        logoImageView.setFitHeight(40);
	        logoImageView.setFitWidth(70);
	        helpPane.setTop(logoImageView);
        }catch(Exception e){
        	System.out.println("ErgoSPARQL_GUI warning: image ck.png was not found");
        }
	        
        // read the help file
        String sparqlHelpString = "";
        InputStream is = null;
        try {
            StringBuffer buf = new StringBuffer();
            is = this.getClass().getResourceAsStream("Ergo/ergo_lib/ergo2sparql/ErgoSPARQL_GUI_Help.txt");
            if (is == null)
                is = new FileInputStream("Ergo/ergo_lib/ergo2sparql/ErgoSPARQL_GUI_Help.txt");
            BufferedReader reader = new BufferedReader(
                                                       new InputStreamReader(is));
            String str;
            if (is != null) {
                while ((str = reader.readLine()) != null) {
                    buf.append(str + "\n");
                }
            }
            sparqlHelpString = buf.toString();
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            try {
                is.close();
            } catch (Exception closeException) {
                closeException.printStackTrace();
            }
        }
        // set the help TextArea with the owlHelpString
        TextArea helpLabel = new TextArea(sparqlHelpString);
        helpLabel.setStyle("-fx-color: lightblue; -fx-border-color: blue; -fx-border-width: 2");
        helpLabel.setWrapText(true);
        helpLabel.setFont(new Font(14));
        helpPane.setCenter(helpLabel);
        GridPane bottomPane = new GridPane();
        bottomPane.add(optionPane, 0, 2);
        bottomPane.setAlignment(Pos.CENTER);
        helpPane.setBottom(bottomPane);
        Scene scene = new Scene(helpPane, 900, 550);
        dialogStage.setScene(scene);
        dialogStage.show();
        // WHAT'S THE USER'S DECISION?
        nextButton.setOnAction(e -> {
                dialogStage.hide();
            });
    }
        
    private final Service service = new Service() {
            @Override
            protected Task createTask() {
                return new Task<Void>() {
                    @Override
                    protected Void call() throws Exception {
                        queryEndpoint();
                        return null;
                    }
                };
            }
        };
        
    public void queryEndpoint() {
        Platform.runLater(new Runnable() {
                @Override
                public void run() {
                    statusLabel.setText("Status: Querying SPARQL endpoint ...");
                }
            });
                
        String sparqlEndpointString = sparqlEndpointTextField.getText();
        String userString = userTextField.getText();
        String passwordString = passwordTextField.getText();
        String irisString = irisTextArea.getText();
        String queryString = queryTextArea.getText();
        String predicateString = predicateTextField.getText();
        if (predicateString.equals(""))
            predicateString = "node";
                
        try{
            // query SPARQL
            ergoText = ErgoSPARQL.queryEndpoint(sparqlEndpointString, userString, passwordString, irisString, queryString, predicateString, outputFile);
                        
            Platform.runLater(new Runnable() {
                    @Override
                    public void run() {
                        ergoTextArea.setText(ergoText);
                        statusLabel.setText("Status: Done querying. Ready.");
                    }
                });
            // save the last successful query
            saveQuery(sparqlDir.getAbsolutePath() + "/lastQuery.properties");
        }catch(IllegalArgumentException e){
            e.printStackTrace();
            Platform.runLater(new Runnable() {
                    @Override
                    public void run() {
                        statusLabel.setText("Status: Error in IRI prefixes");
                    }
                });
        }catch(QueryExceptionHTTP e){
            e.printStackTrace();
            Platform.runLater(new Runnable() {
                    @Override
                    public void run() {
                        statusLabel.setText("Status: SPARQL Host cannot be reached");
                    }
                });
        }catch(IOException e){
            e.printStackTrace();
            Platform.runLater(new Runnable() {
                    @Override
                    public void run() {
                        statusLabel.setText("Status: File output error");
                    }
                });
        }catch(QueryException e){
            e.printStackTrace();
            Platform.runLater(new Runnable() {
                    @Override
                    public void run() {
                        statusLabel.setText("Status: SPARQL Query error" + e.getClass());
                    }
                });
        }
    }

    @Override
    public void stop(){
        // save the last successful query
        saveQuery(sparqlDir.getAbsolutePath() + "/lastQuery.properties");
        log.close();
    }
        
    // SPARQL directory and log
    private static File sparqlDir;
    private static PrintStream log;

    // last inputs
    private static boolean lastPropertiesExist = false;
    private static String lastURL;
    private static String lastUser;
    private static String lastPassword;
    private static String lastPreffixes;
    private static String lastQuery;
    private static String lastOutputFile;
        
    public static void main(String[] args) {
        String rootDir;
                
        // get location of Ergo
        if(args!=null && args.length>0){
            rootDir = args[0];
        }else{
            rootDir = System.getProperty("java.io.tmpdir");
        }

        // SPARQL properties directory
        sparqlDir = new File(rootDir+"/.ergoSparql");
        if (!sparqlDir.exists()) {
            if(sparqlDir.mkdir()) {
                //System.out.println("SPARQL properties directory is created: " + sparqlDir.getAbsolutePath());
            } else {
                System.out.println("SPARQL properties: failed to create directory: " + sparqlDir.getAbsolutePath());
            }
        }
                
        // creating log
        String sparqlLogFile = sparqlDir.getAbsolutePath() + "/sparql.log";
        try{
            log = System.out;
            // log = new PrintWriter(new BufferedWriter(new FileWriter(sparqlLogFile, true)));
        }catch(Exception e){
            e.printStackTrace();
            System.out.println("SPARQL log file cannot be written!");
        }
        Calendar date = new GregorianCalendar();
        log.println("\nDate & time: " + date.getTime());
        log.println("SPARQL properties directory exists: " + sparqlDir.getAbsolutePath());
                
        // load default link, user, prefixes and query
        loadLastQueryFromPropertyFile(sparqlDir.getAbsolutePath() + "/lastQuery.properties");
                
        Logger.getRootLogger().setLevel(Level.OFF);
        launch(args);
    }
}





























