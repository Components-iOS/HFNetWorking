#
# Be sure to run `pod lib lint HFNetWorking.podspec' to ensure this is a
# valid spec before submitting.
#
# Any lines starting with a # are optional, but their use is encouraged
# To learn more about a Podspec see https://guides.cocoapods.org/syntax/podspec.html
#

Pod::Spec.new do |s|
  s.name             = 'HFNetWorking'
  s.version          = '0.1.1'
  s.summary          = '网络请求'
  s.description      = <<-DESC
网络请求工具
                       DESC
  s.homepage         = 'https://github.com/Components-iOS'
  s.license          = { :type => 'MIT', :file => 'LICENSE' }
  s.author           = { 'liuhongfei' => '13718045729@163.com' }
  s.source           = { :git => 'https://github.com/Components-iOS/HFNetWorking.git', :tag => s.version.to_s }
  s.platform     = :ios, "12.0"
  s.ios.deployment_target = '12.0'
  s.requires_arc = true
  
  s.resource_bundles = {
      'HFNetWorking' => ['HFNetWorking/Assets/PrivacyInfo.xcprivacy']
  }
  
  s.source_files = 'HFNetWorking/Classes/**/*'

end
