/*
 * Copyright 2016, Coherent Knowledge Systems.
 *
 * All rights reserved.
 */

package com.coherentknowledge.ergo.owl;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Scanner;
import java.util.Set;

import com.hp.hpl.jena.rdf.model.Model;
import com.hp.hpl.jena.util.FileManager;

/**
 *
 * @author Paul Fodor
 */
public class ErgoOWLAPI_Tests {

    /**
     * Testing method that calls the translator and shows the output.
     * 
     * @param inputFileName
     * @param inputLangSyntax
     */
    public static void translateTest(String inputFileName,
            String inputLangSyntax, String outFileName,
            String outputFormat, String irisText) {
        try {
            //System.out.println("\nBeginning translation for " + inputFileName + "\n");
            boolean result = ErgoOWLAPI.translateRDF(inputFileName,
                    inputLangSyntax, outFileName, outputFormat, irisText);
            //printOutput(inputFileName, outputFormat);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void printOutput(String inputFileName, String outputFormat) {
        try {
            // print the output
            File outputFile = new File(inputFileName
                    + (outputFormat.equals("fastload") ? ".P" : ".ergo"));
            Scanner outputScanner = new Scanner(outputFile);
            while (outputScanner.hasNext()) {
                System.out.println(outputScanner.nextLine());
            }
            // System.out.println("\nDone translation for " + inputFileName +
            // "\n");
            outputScanner.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    public static void main(String[] args) {
        try {
            ArrayList<String> inputFileNames = new ArrayList<String>();
            String inputLangSyntax;

            boolean triplesFlag = false;
            String outputFormat = "fastload";
            String irisText = "";

            inputFileNames.add("./data/BioAgent.rdf");
            inputFileNames.add("./data/BodyCorporate.owl");
            inputFileNames.add("./data/Chars_hop1.nt");
            inputFileNames.add("./data/bfo.owl");
            inputFileNames.add("./data/characteristics.ttl");
            inputFileNames.add("./data/community-label.owl");
            inputFileNames.add("./data/dc-1.1.rdf");
            inputFileNames.add("./data/dl-safe-ancestor.ttl");
            inputFileNames.add("./data/example4.jsonld");
            inputFileNames.add("./data/family.owl");
            inputFileNames.add("./data/food.owl");
            inputFileNames.add("./data/location-mapping.ttl");
            inputFileNames.add("./data/people.owl");
            inputFileNames.add("./data/pets.owl");
            inputFileNames.add("./data/pizza.owl");
            inputFileNames.add("./data/skos.rdf");
            inputFileNames.add("./data/wine.owl");
            inputFileNames.add("./data/wine.rdf");
            inputFileNames.add("./data/wine_g.owl");
            
            inputLangSyntax = "";
            
            //inputLangSyntax = "RDF/XML";
            //inputLangSyntax = "TURTLE";
            //inputLangSyntax = "JSON-LD";
            for(String inputFileName:inputFileNames){
                System.out.println("translating " + inputFileName);                       
                String outputFileName = inputFileName + (outputFormat.equals("fastload") ? ".P" : ".ergo");
                translateTest(inputFileName, inputLangSyntax,
                        outputFileName, outputFormat, irisText);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        System.out.println("ErgoOWL tests done.");       
    }
}
