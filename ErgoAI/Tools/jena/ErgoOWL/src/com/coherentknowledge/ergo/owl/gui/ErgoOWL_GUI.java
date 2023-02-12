/*
 * Copyright 2014, Coherent Knowledge Systems.
 *
 * All rights reserved.
 */
package com.coherentknowledge.ergo.owl.gui;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.URL;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.Scanner;

import org.apache.log4j.Level;
import org.apache.log4j.Logger;

import javafx.application.Application;
import javafx.application.Platform;
import javafx.beans.binding.Bindings;
import javafx.concurrent.Service;
import javafx.concurrent.Task;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.scene.Cursor;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.GridPane;
import javafx.scene.text.Font;
import javafx.stage.DirectoryChooser;
import javafx.stage.FileChooser;
import javafx.stage.Modality;
import javafx.stage.Stage;
import javafx.geometry.Pos;
import javafx.scene.control.Label;
import javafx.scene.control.RadioButton;
import javafx.scene.control.TextArea;
import javafx.scene.control.TextField;
import javafx.scene.control.Toggle;
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

/**
 *
 * @author Paul Fodor
 */
public class ErgoOWL_GUI extends Application {

    String versionString = "0.7.19 (July 19, 2015)";

    Label statusLabel = new Label("");

    // input file variable and name 
    File inputFile;
    String inputFileName = "";
        
    Label inputFileLabel = new Label("Input file: " + inputFileName);

    RadioButton nqFileRadioButton, nqDirRadioButton, 
        xmlFileRadioButton, xmlDirRadioButton, 
        jsonFileRadioButton, jsonDirRadioButton, 
        ttlFileRadioButton, ttlDirRadioButton;
    ToggleGroup inputRadioGroup = new ToggleGroup();

    RadioButton ntriplesRadioButton;
    RadioButton nquadsRadioButton;
    ToggleGroup triplesOrQuadsRadioGroup = new ToggleGroup();

    RadioButton fastloadRadioButton;
    RadioButton predicatesRadioButton;
    RadioButton framesRadioButton;
    ToggleGroup formatRadioGroup = new ToggleGroup();

    TextField graphNameTextField;

    Button translateButton;
        
    // center
    TextArea owlTextArea = new TextArea();
    TextArea ergoTextArea = new TextArea();
    Label owlTopLabel;
    Label ergoTopLabel;

    // programs text
    String ergoText = "";
    String owlString = "";
        
    // IRIs
    TextArea irisTextArea = new TextArea();

    Stage primaryStage;
        
    @Override
    public void start(Stage primaryStage) {      
        this.primaryStage = primaryStage;
        // Create GUI
        MenuBar menuBar = new MenuBar();
        //Menu fileOperations = new Menu("File");
        Menu helpOperations = new Menu("Help");
        menuBar.getMenus().addAll(helpOperations); 

        MenuItem menuItemAbout = new MenuItem("About");
        helpOperations.getItems().addAll(menuItemAbout);
                
        menuItemAbout.setAccelerator(KeyCombination.keyCombination("F1"));              
        menuItemAbout.setOnAction( e -> helpDialog(primaryStage) );
                
        StackPane top = new StackPane();
        
        Label title = new Label("Ergo RDF&OWL Import Tool");
                
        title.setFont(new Font(26));
        top.getChildren().add(title);

        VBox left = new VBox();
        // left.setAlignment(Pos.CENTER);
        Label importLabel = new Label("   Import RDF & OWL");
        importLabel.setFont(new Font(20));
        left.getChildren().add(importLabel);

        // Status label
        FlowPane statusPane = new FlowPane();
        statusLabel.setFont(new Font(20));
        statusLabel.setStyle("-fx-text-fill: red; -fx-font-weight:bolder");
        statusLabel.setText("Status: Ready");
        statusPane.getChildren().addAll(statusLabel);
        left.getChildren().add(statusPane);

        // select input file
        VBox inputPane = new VBox();
        inputPane.setStyle("-fx-color: lightblue; -fx-border-color: blue; -fx-border-width: 2");

        Label inputLabel = new Label("   Select input:");
        inputLabel.setFont(new Font(20));
        inputPane.getChildren().add(inputLabel);

        nqFileRadioButton = new RadioButton("Import RDF/OWL N-triples or N-quads file (.nq, .nt)");
        nqFileRadioButton.setStyle("-fx-color: lightyellow; -fx-border-color: yellow; -fx-border-width: 2");
        nqFileRadioButton.setOnAction(new EventHandler<ActionEvent>(){ public void handle(ActionEvent e){ 
            FileChooser.ExtensionFilter extFilter = new FileChooser.ExtensionFilter("N-triples or N-quads files (*.nt, *.nq)", "*.nt", "*.nq");
            selectFile(extFilter);
        }});
        nqFileRadioButton.setToggleGroup(inputRadioGroup);
        inputPane.getChildren().add(nqFileRadioButton);

        nqDirRadioButton = new RadioButton("Import RDF/OWL N-triples or N-quads directory");
        nqDirRadioButton.setStyle("-fx-color: lightyellow; -fx-border-color: yellow; -fx-border-width: 2");
        nqDirRadioButton.setOnAction(new EventHandler<ActionEvent>(){ public void handle(ActionEvent e){ selectDirectory(); }});
        nqDirRadioButton.setToggleGroup(inputRadioGroup); 
        inputPane.getChildren().add(nqDirRadioButton);

        xmlFileRadioButton = new RadioButton("Import RDF/OWL XML file (.rdf, .owl, .xml)");
        xmlFileRadioButton.setStyle("-fx-color: orange; -fx-border-width: 2");
        xmlFileRadioButton.setOnAction(new EventHandler<ActionEvent>(){ public void handle(ActionEvent e){ 
            FileChooser.ExtensionFilter extFilter = new FileChooser.ExtensionFilter("RDF XML files (*.xml, *.rdf, *.owl)", "*.xml", "*.rdf", "*.owl");
            selectFile(extFilter); 
        }});
        xmlFileRadioButton.setToggleGroup(inputRadioGroup);
        inputPane.getChildren().add(xmlFileRadioButton);

        xmlDirRadioButton = new RadioButton("Import RDF/OWL XML directory");
        xmlDirRadioButton.setStyle("-fx-color: orange; -fx-border-width: 2");
        xmlDirRadioButton.setOnAction(new EventHandler<ActionEvent>(){ public void handle(ActionEvent e){ selectDirectory(); }});
        xmlDirRadioButton.setToggleGroup(inputRadioGroup);
        inputPane.getChildren().add(xmlDirRadioButton);

        jsonFileRadioButton = new RadioButton("Import JSON-LD file (.jsonld)");
        jsonFileRadioButton.setStyle("-fx-color: brown; -fx-border-width: 2");
        jsonFileRadioButton.setOnAction(new EventHandler<ActionEvent>(){ public void handle(ActionEvent e){ 
            FileChooser.ExtensionFilter extFilter = new FileChooser.ExtensionFilter("JSON-LD files (*.jsonld)", "*.jsonld");
            selectFile(extFilter); 
        }});
        jsonFileRadioButton.setToggleGroup(inputRadioGroup);
        inputPane.getChildren().add(jsonFileRadioButton);

        jsonDirRadioButton = new RadioButton("Import JSON-LD directory");
        jsonDirRadioButton.setStyle("-fx-color: brown; -fx-border-width: 2");
        jsonDirRadioButton.setOnAction(new EventHandler<ActionEvent>(){ public void handle(ActionEvent e){ selectDirectory(); }});
        jsonDirRadioButton.setToggleGroup(inputRadioGroup);
        inputPane.getChildren().add(jsonDirRadioButton);

        ttlFileRadioButton = new RadioButton("Import RDF/OWL Turtle file (.ttl)");
        ttlFileRadioButton.setStyle("-fx-color: blue; -fx-border-width: 2");
        ttlFileRadioButton.setOnAction(new EventHandler<ActionEvent>(){ public void handle(ActionEvent e){ 
            FileChooser.ExtensionFilter extFilter = new FileChooser.ExtensionFilter("Turtle files (*.ttl)", "*.ttl");
            selectFile(extFilter); 
        }});
        ttlFileRadioButton.setToggleGroup(inputRadioGroup);
        inputPane.getChildren().add(ttlFileRadioButton);

        ttlDirRadioButton = new RadioButton("Import RDF/OWL Turtle  directory");
        ttlDirRadioButton.setStyle("-fx-color: blue; -fx-border-width: 2");
        ttlDirRadioButton.setOnAction(new EventHandler<ActionEvent>(){ public void handle(ActionEvent e){ selectDirectory(); }});
        ttlDirRadioButton.setToggleGroup(inputRadioGroup);
        inputPane.getChildren().add(ttlDirRadioButton);

        inputFileLabel.setFont(new Font(18));
        inputFileLabel.setStyle("-fx-text-fill: red;");
        inputPane.getChildren().add(inputFileLabel);

        left.getChildren().addAll(inputPane);

        // Select the output
        VBox outputPane = new VBox();
        outputPane.setStyle("-fx-color: lightblue; -fx-border-color: blue; -fx-border-width: 2");

        Label outputLabel = new Label("Output predicate arity (n-quads or n-triples):");
        outputLabel.setFont(new Font(20));

        ntriplesRadioButton = new RadioButton("n-triples");
        nquadsRadioButton = new RadioButton("n-quads");
        ntriplesRadioButton.setToggleGroup(triplesOrQuadsRadioGroup);
        nquadsRadioButton.setToggleGroup(triplesOrQuadsRadioGroup);

        // default format is triples
        ntriplesRadioButton.setSelected(true);
        ErgoOWL.triplesFlag = true;

        ntriplesRadioButton.setOnAction(new EventHandler<ActionEvent>(){ public void handle(ActionEvent e){ ErgoOWL.triplesFlag = true; }});
        nquadsRadioButton.setOnAction(new EventHandler<ActionEvent>(){ public void handle(ActionEvent e){ ErgoOWL.triplesFlag = false; }});

        graphNameTextField = new TextField();
        graphNameTextField.setPromptText("Output quad's graph name ('main' is the default)");

        outputPane.getChildren().addAll(outputLabel, ntriplesRadioButton, nquadsRadioButton, graphNameTextField);

        left.getChildren().addAll(outputPane);

        // Select the output
        VBox outputFormatPane = new VBox();
        outputFormatPane.setStyle("-fx-color: lightblue; -fx-border-color: blue; -fx-border-width: 2");

        Label outputFormatLabel = new Label("Output format (fastload .P or .ergo):");
        outputFormatLabel.setFont(new Font(20));

        fastloadRadioButton = new RadioButton("fastload format");
        predicatesRadioButton = new RadioButton("predicate syntax: p(s,o) or p(s,o,g)");
        framesRadioButton = new RadioButton("frame syntax: s[p->o]");
        fastloadRadioButton.setToggleGroup(formatRadioGroup);
        predicatesRadioButton.setToggleGroup(formatRadioGroup);
        framesRadioButton.setToggleGroup(formatRadioGroup);

        // set the default frames to true
        fastloadRadioButton.setSelected(true);
        ErgoOWL.outputFormat = 1;

        fastloadRadioButton.setOnAction(new EventHandler<ActionEvent>(){ public void handle(ActionEvent e){ ErgoOWL.outputFormat = 1; }});
        predicatesRadioButton.setOnAction(new EventHandler<ActionEvent>(){ public void handle(ActionEvent e){ ErgoOWL.outputFormat = 2; }});
        framesRadioButton.setOnAction(new EventHandler<ActionEvent>(){ public void handle(ActionEvent e){ ErgoOWL.outputFormat = 3; }});

        outputFormatPane.getChildren().addAll(outputFormatLabel, fastloadRadioButton, predicatesRadioButton, framesRadioButton);

        left.getChildren().addAll(outputFormatPane);

        // manage iris
        VBox irisPane = new VBox();
        irisPane.setStyle("-fx-color: lightblue; -fx-border-color: blue; -fx-border-width: 2");

        Label irisLabel = new Label("Manage IRIs:");
        irisLabel.setFont(new Font(20));
        irisPane.getChildren().add(irisLabel);

        irisTextArea.setPrefRowCount(10);
        irisTextArea.setPrefColumnCount(10);
        // initialize iris:
        ErgoOWL.initializeIris();
        irisTextArea.setText(ErgoOWL.getIrisText());
        irisPane.getChildren().add(irisTextArea);

        left.getChildren().add(irisPane);

        // translate
        VBox translatePane = new VBox();
        translatePane.setAlignment(Pos.CENTER);
        translateButton = new Button("Import RDF/OWL");
        translateButton.setStyle("-fx-color: pink; -fx-border-color: red; -fx-border-width: 4");
        translateButton.setFont(new Font(20));
                                
        translateButton.disableProperty().bind(service.runningProperty());
                
        translateButton.setOnAction(
                                    new EventHandler<ActionEvent>(){ 
                                        public void handle(ActionEvent e){ 
                                            service.restart();
                                        }});
        translatePane.getChildren().addAll(translateButton);
                
        left.getChildren().addAll(translatePane);

        // CENTER
        GridPane center = new GridPane();
        // center.setAlignment(Pos.CENTER);
        center.setHgap(10);
        center.setVgap(10);

        owlTopLabel = new Label("Original RDF/OWL file:");
        center.add(owlTopLabel, 0, 0);
        owlTextArea.setPrefRowCount(40);
        // ergoTA.setPrefColumnCount(20);
        center.add(owlTextArea, 0, 1);

        ergoTopLabel = new Label("Ergo file:");
        center.add(ergoTopLabel, 1, 0);
        // ergoTA.setPrefRowCount(50);
        // ergoTA.setPrefColumnCount(20);
        center.add(ergoTextArea, 1, 1);

        // BOTTOM
        FlowPane bottom = new FlowPane();
        // bottom.setPadding(new Insets(11.5, 12.5, 13.5, 14.5));

        try{
	        Image image = new Image(getClass().getResourceAsStream("Ergo/ergo_lib/ergo2owl/ck.png"));
	        ImageView imageView = new ImageView(image);
	        imageView.setFitHeight(50);
	        imageView.setFitWidth(90);
	        bottom.getChildren().addAll(imageView);
        }catch(Exception e){
        	System.out.println("ErgoOWL: ck.png image not loaded");
        }
	        
        Label copyrightVersionLabel = new Label(
                                                "     @Copyright 2015, Coherent Knowledge Systems, Ergo/OWL Import Tool, version " 
                                                + versionString
                                                + "                                                    ");
        copyrightVersionLabel.setFont(new Font(14));
        Button helpButton = new Button("Help");
        helpButton.setStyle("-fx-color: lightgreen; -fx-border-color: green; -fx-border-width: 2");
        helpButton.setTextFill(Color.RED);
        helpButton.setFont(new Font(20));
        helpButton.setOnAction(new EventHandler<ActionEvent>(){ public void handle(ActionEvent e){ helpDialog(primaryStage); }});

        bottom.getChildren().addAll(copyrightVersionLabel);

        // MAIN Pane
        VBox rootNode = new VBox(10);
                
        BorderPane contentNode = new BorderPane();
        contentNode.setTop(top);
        contentNode.setBottom(bottom);
        contentNode.setLeft(left);
        contentNode.setCenter(center);

        rootNode.getChildren().addAll(menuBar, contentNode);
                
        Scene scene = new Scene(rootNode, 1200, 950);

        primaryStage.setTitle("Ergo RDF/OWL");
        primaryStage.setScene(scene);
                
        primaryStage
            .getScene()
            .getRoot()
            .cursorProperty()
            .bind(Bindings.when(service.runningProperty())
                  .then(Cursor.WAIT).otherwise(Cursor.DEFAULT));

        primaryStage.show();
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
        	System.out.println("ErgoOWL: ck.png image not found");
        }
        
        // read the help file
        String owlHelpString = "";
        InputStream is = null;
        try{
            StringBuffer buf = new StringBuffer();
            is = this.getClass().getResourceAsStream("Ergo/ergo_lib/ergo2owl/ErgoOWL_GUI_Help.txt");
            if(is==null)
                is = new FileInputStream("Ergo/ergo_lib/ergo2owl/ErgoOWL_GUI_Help.txt");
            BufferedReader reader = new BufferedReader(new InputStreamReader(is));
            String str;
            if (is!=null) {                         
                while ((str = reader.readLine()) != null) { 
                    buf.append(str + "\n" );
                }               
            }                   
            owlHelpString = buf.toString();
        }catch(Exception e){
            e.printStackTrace();
        } finally {
            try { 
                is.close(); 
            } catch (Exception closeException) {
                closeException.printStackTrace();
            }
        }
        // set the help TextArea with the owlHelpString
        TextArea helpLabel = new TextArea(owlHelpString);
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
        nextButton.setOnAction(new EventHandler<ActionEvent>(){ public void handle(ActionEvent e){ dialogStage.hide(); }});
    }

    // variables for directory input 
    boolean directoryInput = false;
    String currentInputFileName = inputFileName;
        
    public void selectFile(FileChooser.ExtensionFilter extFilter) {
        try{
            FileChooser inputFileChooser = new FileChooser();
            inputFileChooser.getExtensionFilters().add(extFilter);
            FileChooser.ExtensionFilter extFilter2 = new FileChooser.ExtensionFilter("All files", "*.*");
            inputFileChooser.getExtensionFilters().add(extFilter2);
            inputFile = inputFileChooser.showOpenDialog(null);
            inputFileName = inputFile.getName();
            directoryInput = false;
            inputFileLabel.setText("Input file: " + inputFileName);
            statusLabel.setText("Ready to translate: " + inputFileName);
        }catch(Exception e){
            System.out.println("No file was selected in the FileChooser.");
            //e.printStackTrace();
        }
    }

    public void selectDirectory() {
        try{
            DirectoryChooser inputDirectoryChooser = new DirectoryChooser();
            inputFile = inputDirectoryChooser.showDialog(null);
            directoryInput = true;
            inputFileName = inputFile.getName() + "/";
            inputFileLabel.setText("Input directory: " + inputFileName);
            statusLabel.setText("Ready to translate: " + inputFileName);
        }catch(Exception e){
            System.out.println("No directory was selected in the DirectoryChooser.");
            //e.printStackTrace();
        }
    }

    private final Service service = new Service() {
            @Override
            protected Task createTask() {
                return new Task<Void>() {
                    @Override
                    protected Void call() throws Exception {
                        translate();
                        return null;
                    }
                };
            }
        };
        
    public void translate() {

        Platform.runLater(new Runnable() {
                @Override
                public void run() {
                    statusLabel.setText("Translating: " + inputFileName + "...");
                }
            });
                                
        // get the graphName:
        String graphName = graphNameTextField.getText();
        if (graphName.equals(""))
            graphName = "main";

        // save iris
        String irisText = irisTextArea.getText();
        ErgoOWL.setIris(irisText);

        // process the input file
        File lastInputFile = inputFile;

        currentInputFileName = inputFileName;
                
        Toggle t = inputRadioGroup.getSelectedToggle();
        try{
            if(nqFileRadioButton == t){
                ergoText = ErgoOWL.importRDFNQuads(inputFile, graphName);                       
            }else if(nqDirRadioButton == t){
                File[] inputFileArray = inputFile.listFiles();
                for (File currentInputFile : inputFileArray) {
                    currentInputFileName = currentInputFile.getName();
                    String extension = currentInputFileName.substring(currentInputFileName.lastIndexOf("."));
                    if (extension.equals(".nq") || extension.equals(".nt")) {
                        lastInputFile = currentInputFile;
                        ergoText = ErgoOWL.importRDFNQuads(currentInputFile, graphName);
                    }
                }
            }else if(xmlFileRadioButton == t){
                ergoText = ErgoOWL.importRDFXML(inputFile, graphName);
            }else if(xmlDirRadioButton == t){
                File[] inputFileArray = inputFile.listFiles();
                for (File currentInputFile : inputFileArray) {
                    currentInputFileName = currentInputFile.getName();
                    String extension = currentInputFileName.substring(currentInputFileName.lastIndexOf("."));
                    if (extension.equals(".xml") || extension.equals(".rdf") || extension.equals(".owl")) {
                        lastInputFile = currentInputFile;
                        ergoText = ErgoOWL.importRDFXML(currentInputFile, graphName);
                    }
                }
            }else if(jsonFileRadioButton == t){
                ergoText = ErgoOWL.importRDFJSON(inputFile, graphName);
            }else if(jsonDirRadioButton == t){
                File[] inputFileArray = inputFile.listFiles();
                for (File currentInputFile : inputFileArray) {
                    currentInputFileName = currentInputFile.getName();
                    String extension = currentInputFileName.substring(currentInputFileName.lastIndexOf("."));
                    if (extension.equals(".json") || extension.equals(".jsonld")) {
                        lastInputFile = currentInputFile;
                        ergoText = ErgoOWL.importRDFJSON(currentInputFile, graphName);
                    }
                }
            }else if(ttlFileRadioButton == t){
                ergoText = ErgoOWL.importRDFTTL(inputFile, graphName);
            }else if(ttlDirRadioButton == t){
                File[] inputFileArray = inputFile.listFiles();
                for (File currentInputFile : inputFileArray) {
                    currentInputFileName = currentInputFile.getName();
                    String extension = currentInputFileName.substring(currentInputFileName.lastIndexOf("."));
                    if (extension.equals(".ttl")) {
                        lastInputFile = currentInputFile;
                        ergoText = ErgoOWL.importRDFTTL(currentInputFile, graphName);
                    }
                }
            }else{
                //System.out.println("No input file was selected");
            }
        
            // OutTextArea
            Platform.runLater(new Runnable() {
                    @Override
                    public void run() {
                        owlTopLabel.setText("Original RDF/OWL file: " + currentInputFileName);
                        ergoTopLabel.setText("Ergo file: " + currentInputFileName + ".ergo");
                    }
                });
                
            Scanner inputFileScanner = new Scanner(lastInputFile);
            owlString = "";
            int counter = 0;
            while (inputFileScanner.hasNext() && counter < 100) {
                owlString += inputFileScanner.nextLine() + "\n";
                counter++;
            }
            if(counter==100)
                owlString += "...";
            inputFileScanner.close();
            Platform.runLater(new Runnable() {
                    @Override
                    public void run() {
                        owlTextArea.setText(owlString);
                        ergoTextArea.setText(ergoText);
                    }
                });
            Platform.runLater(new Runnable() {
                    @Override
                    public void run() {
                        statusLabel.setText("Status: Done translating " + inputFileName);
                    }
                });
        } catch (Exception e) {
            // e.printStackTrace();
            Platform.runLater(new Runnable() {
                    @Override
                    public void run() {
                        if(inputFile==null)
                            statusLabel.setText("Status: No input file was selected");
                        else if(!inputFile.exists() || !inputFile.canRead())
                            statusLabel.setText("Status: Input file cannot be read");
                        else 
                            statusLabel.setText("Status: Input file error (wrong format)");
                    }
                });
        }
    }

    public static void main(String[] args) {
        Logger.getRootLogger().setLevel(Level.OFF);
        launch(args);
    }
}
