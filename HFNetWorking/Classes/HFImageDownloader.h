// HFImageDownloader.h
// Copyright (c) 2011–2016 Alamofire Software Foundation ( http://alamofire.org/ )
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#import <TargetConditionals.h>

#if TARGET_OS_IOS || TARGET_OS_TV 

#import <Foundation/Foundation.h>
#import "HFAutoPurgingImageCache.h"
#import "HFHTTPSessionManager.h"

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSInteger, HFImageDownloadPrioritization) {
    HFImageDownloadPrioritizationFIFO,
    HFImageDownloadPrioritizationLIFO
};

/**
 The `HFImageDownloadReceipt` is an object vended by the `HFImageDownloader` when starting a data task. It can be used to cancel active tasks running on the `HFImageDownloader` session. As a general rule, image data tasks should be cancelled using the `HFImageDownloadReceipt` instead of calling `cancel` directly on the `task` itself. The `HFImageDownloader` is optimized to handle duplicate task scenarios as well as pending versus active downloads.
 */
@interface HFImageDownloadReceipt : NSObject

/**
 The data task created by the `HFImageDownloader`.
*/
@property (nonatomic, strong) NSURLSessionDataTask *task;

/**
 The unique identifier for the success and failure blocks when duplicate requests are made.
 */
@property (nonatomic, strong) NSUUID *receiptID;
@end

/** The `HFImageDownloader` class is responsible for downloading images in parallel on a prioritized queue. Incoming downloads are added to the front or back of the queue depending on the download prioritization. Each downloaded image is cached in the underlying `NSURLCache` as well as the in-memory image cache. By default, any download request with a cached image equivalent in the image cache will automatically be served the cached image representation.
 */
@interface HFImageDownloader : NSObject

/**
 The image cache used to store all downloaded images in. `HFAutoPurgingImageCache` by default.
 */
@property (nonatomic, strong, nullable) id <HFImageRequestCache> imageCache;

/**
 The `HFHTTPSessionManager` used to download images. By default, this is configured with an `HFImageResponseSerializer`, and a shared `NSURLCache` for all image downloads.
 */
@property (nonatomic, strong) HFHTTPSessionManager *sessionManager;

/**
 Defines the order prioritization of incoming download requests being inserted into the queue. `HFImageDownloadPrioritizationFIFO` by default.
 */
@property (nonatomic, assign) HFImageDownloadPrioritization downloadPrioritization;

/**
 The shared default instance of `HFImageDownloader` initialized with default values.
 */
+ (instancetype)defaultInstance;

/**
 Creates a default `NSURLCache` with common usage parameter values.

 @returns The default `NSURLCache` instance.
 */
+ (NSURLCache *)defaultURLCache;

/**
 The default `NSURLSessionConfiguration` with common usage parameter values.
 */
+ (NSURLSessionConfiguration *)defaultURLSessionConfiguration;

/**
 Default initializer

 @return An instance of `HFImageDownloader` initialized with default values.
 */
- (instancetype)init;

/**
 Initializer with specific `URLSessionConfiguration`
 
 @param configuration The `NSURLSessionConfiguration` to be be used
 
 @return An instance of `HFImageDownloader` initialized with default values and custom `NSURLSessionConfiguration`
 */
- (instancetype)initWithSessionConfiguration:(NSURLSessionConfiguration *)configuration;

/**
 Initializes the `HFImageDownloader` instance with the given session manager, download prioritization, maximum active download count and image cache.

 @param sessionManager The session manager to use to download images.
 @param downloadPrioritization The download prioritization of the download queue.
 @param maximumActiveDownloads  The maximum number of active downloads allowed at any given time. Recommend `4`.
 @param imageCache The image cache used to store all downloaded images in.

 @return The new `HFImageDownloader` instance.
 */
- (instancetype)initWithSessionManager:(HFHTTPSessionManager *)sessionManager
                downloadPrioritization:(HFImageDownloadPrioritization)downloadPrioritization
                maximumActiveDownloads:(NSInteger)maximumActiveDownloads
                            imageCache:(nullable id <HFImageRequestCache>)imageCache;

/**
 Creates a data task using the `sessionManager` instance for the specified URL request.

 If the same data task is already in the queue or currently being downloaded, the success and failure blocks are
 appended to the already existing task. Once the task completes, all success or failure blocks attached to the
 task are executed in the order they were added.

 @param request The URL request.
 @param success A block to be executed when the image data task finishes successfully. This block has no return value and takes three arguments: the request sent from the client, the response received from the server, and the image created from the response data of request. If the image was returned from cache, the response parameter will be `nil`.
 @param failure A block object to be executed when the image data task finishes unsuccessfully, or that finishes successfully. This block has no return value and takes three arguments: the request sent from the client, the response received from the server, and the error object describing the network or parsing error that occurred.

 @return The image download receipt for the data task if available. `nil` if the image is stored in the cache.
 cache and the URL request cache policy allows the cache to be used.
 */
- (nullable HFImageDownloadReceipt *)downloadImageForURLRequest:(NSURLRequest *)request
                                                        success:(nullable void (^)(NSURLRequest *request, NSHTTPURLResponse  * _Nullable response, UIImage *responseObject))success
                                                        failure:(nullable void (^)(NSURLRequest *request, NSHTTPURLResponse * _Nullable response, NSError *error))failure;

/**
 Creates a data task using the `sessionManager` instance for the specified URL request.

 If the same data task is already in the queue or currently being downloaded, the success and failure blocks are
 appended to the already existing task. Once the task completes, all success or failure blocks attached to the
 task are executed in the order they were added.

 @param request The URL request.
 @param receiptID The identifier to use for the download receipt that will be created for this request. This must be a unique identifier that does not represent any other request.
 @param success A block to be executed when the image data task finishes successfully. This block has no return value and takes three arguments: the request sent from the client, the response received from the server, and the image created from the response data of request. If the image was returned from cache, the response parameter will be `nil`.
 @param failure A block object to be executed when the image data task finishes unsuccessfully, or that finishes successfully. This block has no return value and takes three arguments: the request sent from the client, the response received from the server, and the error object describing the network or parsing error that occurred.

 @return The image download receipt for the data task if available. `nil` if the image is stored in the cache.
 cache and the URL request cache policy allows the cache to be used.
 */
- (nullable HFImageDownloadReceipt *)downloadImageForURLRequest:(NSURLRequest *)request
                                                 withReceiptID:(NSUUID *)receiptID
                                                        success:(nullable void (^)(NSURLRequest *request, NSHTTPURLResponse  * _Nullable response, UIImage *responseObject))success
                                                        failure:(nullable void (^)(NSURLRequest *request, NSHTTPURLResponse * _Nullable response, NSError *error))failure;

/**
 Cancels the data task in the receipt by removing the corresponding success and failure blocks and cancelling the data task if necessary.

 If the data task is pending in the queue, it will be cancelled if no other success and failure blocks are registered with the data task. If the data task is currently executing or is already completed, the success and failure blocks are removed and will not be called when the task finishes.

 @param imageDownloadReceipt The image download receipt to cancel.
 */
- (void)cancelTaskForImageDownloadReceipt:(HFImageDownloadReceipt *)imageDownloadReceipt;

@end

#endif

NS_ASSUME_NONNULL_END
