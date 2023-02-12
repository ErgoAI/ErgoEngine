<?php
	// counter.php the program to show the counter number.
	
	function CounterImage() {
	   $counterLogFile = "/tmp/persistent/flora/.counter.log";
	   
	   if (file_exists($counterLogFile) == true) {
	      if(($fp= fopen($counterLogFile, "r+")) == false) {
	         printf("fopen of the file %s failed\n", $counterLogFile);
	         exit;
	      }
	      if (($content = fread($fp, filesize($counterLogFile))) == false) {
	         printf(" fread failed on the file %s\n", $counterLogFile);
	         exit;
	      } else {
		 $content=chop($content); //trim the return character
	         if (($imageLocation = convertToImage($content)) == false) {
	            printf("ConverttoImage failed\n");
	            exit;
	         } else {
	            $content++;
	            if (rewind($fp) == 0) {
	               printf("rewind failed\n");
	               exit;
	            }
	            if (!fwrite($fp, $content, strlen($content))) {
	               printf("fwrite failed while updateing count in the file %s\n", $counterLogFile);
	               exit;
	            }
	            return $imageLocation;
	         }
	      }
	   }else {
	      if (($fp = fopen($counterLogFile, "w")) ==false) {
	         printf("fopen of the file %s failed\n", $counterLogFile);
	         exit;
	      }
	      if (!fwrite($fp, "1", 1)) {
	         printf("fwrite failed on the file %s\n", $counterLogFile);
	         exit;
	      }
	   }
	}


	function ConvertToImage($content) {
	   
	   $imageFile = "/tmp/persistent/flora/.counter.png";
	   $relativePath = ".counter.png";
	   $noOfChars = strlen($content);
	   
	   $charHeight = ImageFontHeight(5);
	   $charWidth = ImageFontWidth(5);
	   $strWidth = $charWidth * $noOfChars;
	   $strHeight = $charHeight;
	   
	   //15 padding
	   $imgWidth = $strWidth + 15;
	   $imgHeight = $strHeight + 15;
	   $imgCenterX = $imgWidth /2;
	   $imgCenterY = $imgHeight /2;
	   
	   $im = ImageCreate($imgWidth, $imgHeight);
	   $script = ImageColorAllocate($im, 0, 255, 0);
	   $outercolor = ImageColorAllocate($im, 99, 140, 214);
	   $innercolor = ImageColorAllocate($im, 0, 0, 0);
	   ImageFilledRectangle($im, 0, 0, $imgWidth, $imgHeight, $outercolor);
	   ImageFilledRectangle($im, 3, 3, $imgWidth -4, $imgHeight-4, $innercolor);
	   
	   //draw string
	   $drawPosX = $imgCenterX - ($strWidth /2) +1;
	   $drawPosY = $imgCenterY - ($strHeight /2);
	   ImageString($im, 5, $drawPosX, $drawPosY, $content, $script);
	   
	   //save image and return
	   ImagePNG($im, $imageFile);
	   return $relativePath;
	}
?>    
